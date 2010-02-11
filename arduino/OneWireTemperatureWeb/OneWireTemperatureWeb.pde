#include <enc28j60.h>
#include <etherShield.h>
#include <ip_arp_udp_tcp.h>
#include <ip_config.h>
#include <net.h>
#include <websrv_help_functions.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONEWIRE 3

OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);
int last_measurement = 0;

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

void setup()   {                
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

  if(millis() > last_measurement + 1000)
  {
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
    }
    
    Serial.println(data);
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
