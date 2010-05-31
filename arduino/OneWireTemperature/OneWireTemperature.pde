#include <OneWire.h>
#include <DallasTemperature.h>

#define ONEWIRE 3

OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);

#define BUFFER_SIZE 550
char data[BUFFER_SIZE + 1];

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

void setup()
{                
  Serial.begin(115200);
  sensors.begin();
}

void loop()                     
{
  int i, j, data_inset;
  float t;
  DeviceAddress addr;
  char float_conv[10];
  
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
  delay(1000);
}
