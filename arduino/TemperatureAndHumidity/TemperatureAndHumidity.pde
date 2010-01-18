// Temperature and humidity sensor node. Based on the FridgeControlWeb project of mine
// as well as http://www.phanderson.com/picaxe/rh_count.html

#include <enc28j60.h>
#include <etherShield.h>
#include <ip_arp_udp_tcp.h>
#include <ip_config.h>
#include <net.h>
#include <websrv_help_functions.h>
#include <avr/io.h>
#include <math.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONEWIRE 3
#define HS1101DATA 5
#define HS1101POWER 7

int count_transitions(int ms);

// How long between measurement cycles
#define SLEEP_SEC 10

OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);

unsigned long last_checked = 0, this_check = 0;

// Web server setup
#define MYWWWPORT 80
#define BUFFER_SIZE 550
#define ERROR_500 "HTTP/1.0 500 Error\r\nContent-Type: text/html\r\n\r\n<h1>500 Error</h1>"

static uint8_t mymac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24}; 
static uint8_t myip[4] = {192, 168, 1, 252};
static uint8_t buf[BUFFER_SIZE + 1];
char data[BUFFER_SIZE + 1];

// The ethernet shield
EtherShield es = EtherShield();

uint16_t http200ok(void)
{
  return(es.ES_fill_tcp_data_p(buf, 0, PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n"
                                            "Pragma: no-cache\r\n\r\n")));
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf)
{
  uint16_t plen;
  plen = http200ok();
  plen = es.ES_fill_tcp_data_p(buf, plen, PSTR("<html><head><title>Temperature sensor</title>"
                                               "</head><body><pre>"));
  plen = es.ES_fill_tcp_data(buf, plen, data);
  plen = es.ES_fill_tcp_data_p(buf, plen, PSTR("</pre></body></html>"));

  return(plen);
}

// Float support is hard on arduinos
// (http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1164927646) with tweaks
char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  char *ret = a;
  long heiltal = (long)f;
  
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

int measure_RH(void)
{
     long RH_count;
     float RH_raw;

     digitalWrite(HS1101POWER, HIGH);   // power up the 555 cicuit
     delay(500);   // allow some time for the 555 to stabilize

     RH_count = count_transitions(1000);
     Serial.println(RH_count);
     digitalWrite(HS1101POWER, LOW); // turn off the 555
     return(RH_count);
}

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

int count_transitions(int ms)
{
     // configure Counter 1
     cbi(TCCR1A, WGM11);
     cbi(TCCR1A, WGM10);

     cbi(TCCR1B, WGM12);  // WGM12::WGM10 000 - Normal mode

     sbi(TCCR1B, CS12);   // CS12::CS10 111 - External clock, count on rising edge.
     sbi(TCCR1B, CS11);
     sbi(TCCR1B, CS10);

     TCNT1 = 0x0000;      // note that TCNT1 is 16-bits
     delay(ms);
     // not sure if should turn off the counter
     return(TCNT1);
}

float calc_RH_corrected(float RH_raw, float Tc)
{
    float T_diff, RH_corrected;

    T_diff = Tc - 25.00;
    RH_corrected = (1.0 + 0.001 * T_diff) * RH_raw;
    return(RH_corrected);
}

void setup()   {                
  // initialize the digital pin as an output:
  Serial.begin(9600);
  sensors.begin();
  
  es.ES_enc28j60Init(mymac);
  es.ES_init_ip_arp_udp_tcp(mymac, myip, MYWWWPORT);
  
  pinMode(HS1101POWER, HIGH);
}

void loop()                     
{
  int i, j, data_inset;
  char float_conv[10];
  float t;
  DeviceAddress addr;
  uint16_t plen, dat_p;
  float RH_raw, RH_corrected;
  int RH_count;

  // Read temperatures, we dump the state to a buffer so we can serve it
  this_check = millis();
  if(this_check > last_checked + SLEEP_SEC * 1000)
  {
    data_inset = 0;
    sensors.requestTemperatures();
    sprintf(data + data_inset, "Sensor count: %d\n",
            sensors.getDeviceCount());
    data_inset = strlen(data);
 
    for(i = 0; i < sensors.getDeviceCount(); i++)
    {
      t = sensors.getTempCByIndex(i);
      sensors.getAddress(addr, i);
      
      data[data_inset++] = 't';
      for (j = 0; j < 8; j++)
      {
        sprintf(data + data_inset, "%02x", addr[j]);
        data_inset += 2;
      }

      sprintf(data + data_inset, ": %s\n", ftoa(float_conv, t, 2));
      data_inset = strlen(data);
      
      RH_count = measure_RH();
      sprintf(data + data_inset, "HS1101 cycles: %d\n", RH_count);
      data_inset = strlen(data);
      
      RH_raw = 557.7 - 0.0759 * RH_count;
      sprintf(data + data_inset, "Raw humidity: %s\n", ftoa(float_conv, RH_raw, 2));
      data_inset = strlen(data);
      
      RH_corrected = calc_RH_corrected(RH_raw, t);
      sprintf(data + data_inset, "Corrected humidity: %s\n",
              ftoa(float_conv, RH_corrected, 2));
      
      Serial.println(data);
    }
    
    last_checked = this_check;
  }
  
  // Handle network packets
  dat_p = es.ES_packetloop_icmp_tcp(buf, es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf));
  if(dat_p != 0)
  {
    if (strncmp("GET ", (char *)&(buf[dat_p]), 4) != 0){
      // head, post and other methods:
      dat_p = http200ok();
      dat_p = es.ES_fill_tcp_data_p(buf, dat_p, PSTR("<h1>200 OK</h1>"));
    }
    
    // just one web page in the "root directory" of the web server
    else if (strncmp("/ ", (char *)&(buf[dat_p+4]), 2) == 0){
      dat_p = print_webpage(buf);
      Serial.println("Served temperature web page");
    }
    
    else{
      dat_p = es.ES_fill_tcp_data_p(buf, 0, PSTR(ERROR_500));
    }
    
    es.ES_www_server_reply(buf, dat_p);
  }
}