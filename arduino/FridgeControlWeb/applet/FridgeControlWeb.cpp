#include <enc28j60.h>
#include <etherShield.h>
#include <ip_arp_udp_tcp.h>
#include <ip_config.h>
#include <net.h>
#include <websrv_help_functions.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// Temperature sensor and compressor setup
#define COMPRESSOR 13
#define ONEWIRE 3

#define HIGHTEMP 4
#define LOWTEMP 3.6

// 220L Kelvinator is 85 watts
#define COMPRESSOR_WATTAGE 85.0

#define SLEEP_SEC 10

#include "WProgram.h"
uint16_t http200ok(void);
uint16_t print_webpage(uint8_t *buf);
char *ftoa(char *a, double f, int precision);
void setup();
void loop();
OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);

unsigned long runtime = 0, chilltime = 0, last_checked = 0, this_check = 0;
uint8_t compressor = LOW;

// Web server setup
#define MYWWWPORT 80
#define BUFFER_SIZE 550
#define ERROR_500 "HTTP/1.0 500 Error\r\nContent-Type: text/html\r\n\r\n<h1>500 Error</h1>"

static uint8_t mymac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24}; 
static uint8_t myip[4] = {192, 168, 1, 253};
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
// (http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1164927646)
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

void setup()   {                
  // initialize the digital pin as an output:
  pinMode(COMPRESSOR, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
  
  es.ES_enc28j60Init(mymac);
  es.ES_init_ip_arp_udp_tcp(mymac, myip, MYWWWPORT);
}

void loop()                     
{
  int i, j, data_inset, delta;
  char float_conv[10];
  float t;
  DeviceAddress addr;
  uint16_t plen, dat_p;

  // Read temperatures, we dump the state to a buffer so we can serve it
  this_check = millis();
  if(this_check > last_checked + SLEEP_SEC * 1000)
  {
    delta = int((this_check - last_checked) / 1000);
    runtime += delta;
    if(compressor == HIGH) chilltime += delta;
    
    data_inset = 0;
    sensors.requestTemperatures();
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
    }
    
    // Control compressor
    if(t > HIGHTEMP && compressor != HIGH){
      compressor = HIGH;
      Serial.println("Asserting the compressor on");
      digitalWrite(COMPRESSOR, compressor);
    }
    else if(t < LOWTEMP && compressor != LOW){
      compressor = LOW;
      Serial.println("Asserting the compressor off");
      digitalWrite(COMPRESSOR, compressor);
    }
 
    // Status dump
    sprintf(data + data_inset,
            "Compressor: %s\nRuntime: %lu\nChilltime: %lu\n%% chill: %d\nWatt hours: %d\n",
            compressor == HIGH ? "on" : "off", runtime, chilltime,
            int(chilltime * 100.0 / runtime),
            int(chilltime * COMPRESSOR_WATTAGE / 3600));
    Serial.println(data);
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

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

