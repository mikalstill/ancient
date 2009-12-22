#include <OneWire.h>
#include <DallasTemperature.h>

#define COMPRESSOR 9
#define ONEWIRE 3

#define HIGHTEMP 4
#define LOWTEMP 3

#define SLEEP_SEC 1

// 220L Kelvinator is 85 watts
#define COMPRESSOR_WATTAGE 85.0

OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);

unsigned long runtime = 0, chilltime = 0;
boolean compressor = false;

void setup()   {                
  // initialize the digital pin as an output:
  pinMode(COMPRESSOR, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
}

void loop()                     
{
  int i;
  float temperature;
  DeviceAddress addr;

  sensors.requestTemperatures();
  
  for(i = 0; i < sensors.getDeviceCount(); i++)
  {
    temperature = sensors.getTempCByIndex(i);
    Serial.print("Current temperature at ");
    sensors.getAddress(addr, 1);
    printAddress(addr);
    Serial.print(" is: ");
    Serial.println(temperature);
  }
  
  if(temperature > HIGHTEMP)
  {
    digitalWrite(COMPRESSOR, HIGH);
    if(!compressor)
    {
      Serial.println("Compressor on");
      compressor = true;
    }
  }
  else if(temperature < LOWTEMP)
  {
    digitalWrite(COMPRESSOR, LOW);
    if(compressor)
    {
      Serial.println("Compressor off");
      compressor = false;
    }
  }

  delay(SLEEP_SEC * 1000);
  runtime += SLEEP_SEC;
  if(compressor) chilltime += SLEEP_SEC;
  
  Serial.print("Efficiency: Total runtime = ");
  Serial.print(runtime);
  Serial.print(", Chill time = ");
  Serial.print(chilltime);
  Serial.print(" (");
  Serial.print(chilltime * 100 / runtime);
  Serial.print("%, ");
  
  // The compressor wattage is consumption for an hour, so work
  // out how many hours we've been operating for. Then divide by
  // 1000 to get kWh.
  Serial.print(chilltime * COMPRESSOR_WATTAGE / 3600 / 1000);
  Serial.println("kWh)");
  
  Serial.println("");
}

// Function to print a one wire device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
