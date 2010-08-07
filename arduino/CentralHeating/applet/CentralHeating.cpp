#include <stdio.h>
#include <string.h>

#include <DS1302.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#define MYSLAVEID    1

#define RTC_R        13
#define RTC_IO       12
#define RTC_CLK      11
#define LCD_RS        7
#define LCD_E         8
#define LCDD4         5
#define LCDD5         4
#define LCDD6        14
#define LCDD7         3

#define HEATER       10
#define ONEWIRE       9

#define BUTTON1      15
#define BUTTON2      16
#define BUTTON3      17
#define BUTTON4      18

#include "WProgram.h"
void SerialOutput(char *s);
char *ftoa(char *a, double f, int precision);
void setup();
uint8_t boundedStep(uint8_t start, uint8_t minimum, uint8_t maximum, uint8_t stepsize);
void loop();
DS1302 rtc(RTC_R, RTC_IO, RTC_CLK);
OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(LCD_RS, LCD_E, LCDD4, LCDD5, LCDD6, LCDD7);

#define BUFFER_SIZE 16
char data[BUFFER_SIZE + 1];
char menu[BUFFER_SIZE + 1];
char prev_data[BUFFER_SIZE + 1] = "";
char prev_menu[BUFFER_SIZE + 1] = "";

#define SERIAL_BUFFER_SIZE 2048
char serial_out[SERIAL_BUFFER_SIZE];
int serial_inset = 0;
char line_buffer[100];

uint8_t setpoint = 18;
float temp = -1000.0;
unsigned long last_temp = 0, heater_on = 0;
uint8_t mode = 0, prev_mode;

uint8_t num_progs = 0, prev_prog = -1, heater_state = LOW;
typedef struct program_internal
{
  uint8_t hour;
  uint8_t minute;
  uint8_t setpoint;
} program;
program progs[10];

char *dow[] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

void SerialOutput(char *s)
{
  char *inset;
  Serial.print("Writing: ");
  Serial.print(s);
  
//  while(serial_inset + strlen(s) > SERIAL_BUFFER_SIZE)
//  {
//    inset = strchr(serial_out, '\n');
//    strcpy(serial_out, inset + 1);
//    serial_inset = strlen(serial_out);
//  }
//  
//  sprintf(serial_out + serial_inset, s);
//  serial_inset = strlen(serial_out);
//  Serial.print("Serial inset: ");
//  Serial.println(serial_inset);
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

void setup()
{
  uint8_t i;
  
  Serial.begin(9600);
  sensors.begin();
  lcd.begin(16, 2);
  
  pinMode(HEATER, OUTPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  
  if(rtc.read_register(0x20) != 0xa3)
  {
    rtc.write_protect(false);
    rtc.halt(false);
    Time t(2010, 6, 19, 22, 10, 0, 7);
    rtc.time(t);
    rtc.write_register(0x20, 0xa3);
  }

  if(rtc.read_register(0x20) == 0xa3)
  {
    Serial.println("** RTC chip present\n");
    rtc.write_register(0x09, 169);
  }
  
  if(EEPROM.read(0) != 0xa3)
  {
    Serial.println("** EEPROM is not initialized\n");

    EEPROM.write(0, 0xa3);    // Signature
    EEPROM.write(1, 0x04);    // Number of programs
    num_progs = 4;
    
    progs[0].hour = 6;
    progs[0].minute = 45;
    progs[0].setpoint = 20;
    
    progs[1].hour = 8;
    progs[1].minute = 45;
    progs[1].setpoint = 0;
    
    progs[2].hour = 17;
    progs[2].minute = 0;
    progs[2].setpoint = 20;
    
    progs[3].hour = 21;
    progs[3].minute = 45;
    progs[3].setpoint = 12;
    
    for(i = 0; i < sizeof(program) * num_progs; i++)
    {
      EEPROM.write(2 + i, ((uint8_t *) progs)[i]);
    }
  }
  else
  {
    Serial.println("** Reading EEPROM\n");
    num_progs = EEPROM.read(1);
    for(i = 0; i < sizeof(program) * num_progs; i++)
      ((uint8_t *) progs)[i] = EEPROM.read(2 + i);
  }
  
  snprintf(line_buffer, 100, "** Programs (%d):\n", num_progs);
  Serial.println(line_buffer);
//  for(i = 0; i < num_progs; i++)
//  {
//    snprintf(line_buffer, 100, "** %d -> %d:%d %d\n", i,
//             progs[i].hour, progs[i].minute, progs[i].setpoint);
//    Serial.println(line_buffer);
//  }
}

// e.g. minutes would be (x, 0, 59, 1)
uint8_t boundedStep(uint8_t start, uint8_t minimum, uint8_t maximum, uint8_t stepsize)
{
  if(start + stepsize < minimum) return maximum;
  if(start + stepsize > maximum) return minimum;
  return start + stepsize;
}

void loop()                     
{
  int i, j, now_minutes, prog_minutes;
  uint8_t current_prog;
  byte buttons = 0;
  char float_conv[10];
  bool weekend = false;
  
  // Determine button state
  if(digitalRead(BUTTON4) == HIGH) buttons += 8;
  if(digitalRead(BUTTON3) == HIGH) buttons += 4;
  if(digitalRead(BUTTON2) == HIGH) buttons += 2;
  if(digitalRead(BUTTON1) == HIGH) buttons += 1;
  
  if(buttons > 0)
  {
    snprintf(line_buffer, 100, "** Button events: %d\n", buttons);
    Serial.println(line_buffer);
  }
  
  // Paint menu
  switch(mode)
  {
    case 0:
    case 1:
      // Default mode
      if(buttons == 1){
        if(mode == 0){
          mode = 1;
          setpoint = 0;
        }
        else{
          mode = 0;
          prev_prog = -1;
        }
      }
      else if(buttons == 2 && setpoint > 0) setpoint -= 1;
      else if(buttons == 4) setpoint += 1;
      else if(buttons == 8){
        prev_mode = mode;
        mode = 2;
      }
      break;
      
    case 2:
      if(buttons == 1) mode = 3;
      else if(buttons == 4) mode = 44;
      else if(buttons == 8) mode = prev_mode;
      break;
      
    case 3:
      if(buttons == 2)
      {
        uint8_t day = rtc.day();
        if(day == 1) day = 7;
        else day -= 1;
        rtc.day(day);
      }
      else if(buttons == 4)
      {
        uint8_t day = rtc.day();
        if(day == 7) day = 1;
        else day += 1;
        rtc.day(day);
      }
      else if(buttons == 8) mode = 4;
      break;
      
    case 4:
      if(buttons == 2) rtc.hour(boundedStep(rtc.hour(), 0, 23, -1));
      else if(buttons == 4) rtc.hour(boundedStep(rtc.hour(), 0, 23, 1));
      else if(buttons == 8) mode = 5;
      break;
      
    case 5:
      if(buttons == 2)
      {
        uint8_t minutes = rtc.minutes();
        if(minutes == 0) minutes = 59;
        else minutes -= 1;
        rtc.minutes(minutes);
      }
      else if(buttons == 4)
      {
        uint8_t minutes = rtc.minutes();
        if(minutes == 59) minutes = 0;
        else minutes += 1;
        rtc.minutes(minutes);
      }
      else if(buttons == 8) mode = prev_mode;
      break;
  }
  
  switch(mode)
  {
    case 0: sprintf(menu, "Off  -   +   Set"); break;
    case 1: sprintf(menu, " On  -   +   Set"); break;
    case 2: sprintf(menu, "Time    Prog  OK"); break;
    case 3: sprintf(menu, "Day  -   +    OK"); break;
    case 4: sprintf(menu, "Hour -   +    OK"); break;
    case 5: sprintf(menu, "Mins -   +    OK"); break;
  }  
  
  // Determine the time
  Time time = rtc.time();
  if(time.day == 1 || time.day == 7) weekend = true;
  
  // Have we changed program? We assume the programs are sorted
  current_prog = num_progs - 1;
  now_minutes = time.hr * 60 + time.min;
  for(i = 0; i < num_progs; i++)
  {
    prog_minutes = progs[i].hour * 60 + progs[i].minute;
    if(now_minutes >= prog_minutes)
    {
      current_prog = i;
    }
    else break;
  }
  
  if(current_prog != prev_prog)
  {
    setpoint = progs[current_prog].setpoint;
    snprintf(line_buffer, 100,
             "** Program %d matches (%d vs %d) -- setpoint %d\n",
             i, progs[current_prog].hour, progs[current_prog].minute,
             setpoint);
    Serial.println(line_buffer);
    prev_prog = current_prog;
  }
  
  // Determine the temperature
  if(millis() - last_temp > 5000 || temp < -200.0)
  {
    Serial.println("** Read temperature\n");
    sensors.requestTemperatures();
    while(sensors.getDeviceCount() == 0)
    {
      sensors.begin();
    }
   
    temp = 0.0;
    for(i = 0; i < sensors.getDeviceCount(); i++)
      temp += sensors.getTempCByIndex(i);
    temp /= sensors.getDeviceCount();
    
    Serial.println(temp);
    //snprintf(line_buffer, 100, "Inside heater controller: %s\n",
    //         ftoa(float_conv, temp, 2);
    //Serial.println(line_buffer);
    
    last_temp = millis();
  }
  
  // Check the temperature
  if(temp < setpoint && setpoint - temp > 0.25)
  {
    if(heater_state != HIGH){
      Serial.println("** Heater on\n");
      heater_on = millis();
    }
    heater_state = HIGH;
  }
  else if(millis() - heater_on > 300000)
  {
    if(heater_state != LOW){
      Serial.println("** Heater off\n");
      snprintf(line_buffer, 100, "Heater run time: %ul\n",
               (millis() - heater_on));
      Serial.println(line_buffer);
    }
    heater_state = LOW;
  }
  digitalWrite(HEATER, heater_state);
    
  // Display only if the screen has changed
  sprintf(data, "%s %02d:%02d N%02d S%02d", 
          dow[time.day - 1], time.hr, time.min,
          (int) temp, setpoint);
  
  if(strcmp(data, prev_data) != 0 || strcmp(menu, prev_menu) != 0)
  {
    Serial.println(">> ");
    Serial.println(data);
    Serial.println("\n");
    lcd.setCursor(0, 0);
    lcd.print(data);
    strcpy(prev_data, data);
    
    Serial.println(">> ");
    Serial.println(menu);
    Serial.println("\n");
    lcd.setCursor(0, 1);
    lcd.print(menu);
    strcpy(prev_menu, menu);
  }
  
  // Poor man's debounce
  if(buttons > 0) delay(100);
}

int main(void)
{
	init();

	setup();
    
	for (;;)
		loop();
        
	return 0;
}

