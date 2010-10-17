//
// rev 1.1 - added proper keyboard handling - good debounce
//          -  removed all serial output in anticipation of incorporating protocol support
// rev 1.2 - added serial receive routines....Added support for some receive commands.
// rev 1.3 - added support for serial programming commands (still to do:  ST, SP
//

#include <stdio.h>
#include <string.h>

#include <DS1302.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#define MYSLAVEID    01
#define DEBOUNCE     10  // Debounce delay
#define SERIALNUMBER 27

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

// button stores
byte buttons[] = {
  15, 16, 17, 18};
#define NUMBUTTONS 4
// track if a button was pressed, released, or is currently pressed 
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

DS1302 rtc(RTC_R, RTC_IO, RTC_CLK);
OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(LCD_RS, LCD_E, LCDD4, LCDD5, LCDD6, LCDD7);

#define BUFFER_SIZE 16
char data[BUFFER_SIZE + 1];
char menu[BUFFER_SIZE + 1];
char prev_data[BUFFER_SIZE + 1] = "";
char prev_menu[BUFFER_SIZE + 1] = "";

int slave_id = MYSLAVEID;
char MSG_ID[2];
int serialnumber=SERIALNUMBER;

#define SERIAL_OUT_BUFFER_SIZE 255
#define SERIAL_IN_BUFFER_SIZE 80
char serial_out[SERIAL_OUT_BUFFER_SIZE];
char serial_in[SERIAL_IN_BUFFER_SIZE]; // a buffer to place data we got from the bus
int serial_read = 0; // the current position of the serial read buffer
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
  uint8_t days;  // bit 0=Sunday,1=Mon,2=Tues etc
} 
program;
program progs[10];

char *dow[] = {
  "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};

void SerialOutput(char *s)
{
  char *inset;
  //  Serial.print("Writing: ");
  //  Serial.print(s);

  while(serial_inset + strlen(s) > SERIAL_OUT_BUFFER_SIZE)
  {
    inset = strchr(serial_out, '\n');
    strcpy(serial_out, inset + 1);
    serial_inset = strlen(serial_out);
  }

  sprintf(serial_out + serial_inset, s);
  serial_inset = strlen(serial_out);
  //  Serial.print("Serial inset: ");
  Serial.println(serial_inset);
}

// Float support is hard on arduinos
// (http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1164927646) with tweaks
char *ftoa(char *a, double f, int precision)
{
  long p[] = {
    0,10,100,1000,10000,100000,1000000,10000000,100000000    };
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

  // make the button pins input
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }

  // Button pinput is handled by an interrupt routine - 
  // Run timer2 interrupt every 15 ms 
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;
  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;


  if(rtc.read_register(0x20) != 0xa5)
  {
    rtc.write_protect(false);
    rtc.halt(false);
    Time t(2010, 6, 19, 22, 10, 0, 7);
    rtc.time(t);
    rtc.write_register(0x20, 0xa5);
  }

  if(rtc.read_register(0x20) == 0xa5)
  {
    Serial.println("** RTC chip present");
    rtc.write_register(0x09, 169);
  } 
  else Serial.println("** RTC chip absent");

  if(EEPROM.read(0) != 0xa5)
  {
    Serial.println("** EEPROM is not initialized");

    EEPROM.write(0, 0xa5);    // Signature
    EEPROM.write(1, 0x04);    // Number of programs
    num_progs = 4;

    progs[0].hour = 6;
    progs[0].minute = 45;
    progs[0].setpoint = 20;
    progs[0].days = B01111111;

    progs[1].hour = 8;
    progs[1].minute = 45;
    progs[1].setpoint = 0;
    progs[1].days = B01111111;

    progs[2].hour = 17;
    progs[2].minute = 0;
    progs[2].setpoint = 20;
    progs[2].days = B01111111;    

    progs[3].hour = 21;
    progs[3].minute = 45;
    progs[3].setpoint = 12;
    progs[3].days = B01111111;

    for(i = 0; i < sizeof(program) * num_progs; i++)
    {
      EEPROM.write(2 + i, ((uint8_t *) progs)[i]);
    }
  }
  else
  {
    Serial.println("** Reading EEPROM");
    num_progs = EEPROM.read(1);
    for(i = 0; i < sizeof(program) * num_progs; i++)
      ((uint8_t *) progs)[i] = EEPROM.read(2 + i);
  }

  //snprintf(line_buffer, 100, "** Programs (%d):\n", num_progs);
  //Serial.println(line_buffer);
  //  for(i = 0; i < num_progs; i++)
  //  {
  //    snprintf(line_buffer, 100, "** %d -> %d:%d %d\n", i,
  //             progs[i].hour, progs[i].minute, progs[i].setpoint);
  //    Serial.println(line_buffer);
  //  }
}


// interrupt handling function to read buttons - just calls check_switches()
SIGNAL(TIMER2_OVF_vect) {
  check_switches();
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis() < lasttime) {
    // we wrapped around, lets just try again
    lasttime = millis();
  }

  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return;
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();

  for (index = 0; index < NUMBUTTONS; index++) {

    currentstate[index] = digitalRead(buttons[index]);   // read the button

    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
        // just pressed
        justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
        // just released
        justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
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

  Time time = rtc.time();

  // While data is available from the serial link, load it into a buffer
  // he he... - remember abour buffer over runs!
  while (Serial.available()>0){
    if (serial_read<SERIAL_IN_BUFFER_SIZE) 
      serial_in[serial_read++]=Serial.read(); // load the input buffer
    else  serial_read=0;  // if there is no space for the string to fit, 
    // then reset the insertion pointer - we have overflowed


  } 

  // see if we need to process an incomming serial command
  //
  if (serial_in[serial_read-1]=='.') // using . for EOL char for test - make <cr> for prod
  {
    // was the message directed at us? (first 2 chars = our ID, and 3rd char ':')
    MSG_ID[0] = serial_in[0]; 
    MSG_ID[1]=serial_in[1]; 
    MSG_ID[2]='\0';
    if (( atoi(MSG_ID) == slave_id ) & (serial_in[2]==':'))
    { 
      //Serial.println("ID Matched");

      // 
      //  Test for and Process read commands
      //
      if (serial_in[3]=='I' & serial_in[4]=='D' & serial_in[5]==' ')  // ID Command?
      {
        sprintf(serial_out, "00:%02d OK %02d", (int) slave_id, 0); // <- todo calculate checksum
        Serial.println(serial_out);
      }

      if (serial_in[3]=='R' & serial_in[4]=='I' & serial_in[5]==' ')  // Read Ident command?
      {
        sprintf(serial_out, "00:Thermostat v1.0 Serial %04d", (int) serialnumber); // <- todo calculate checksum
        Serial.println(serial_out);
      }  

      if (serial_in[3]=='R' & serial_in[4]=='C' & serial_in[5]==' ')  // Read Current Temp command?
      {
        sprintf(serial_out, "00:+%02d", (int)temp); // <- todo calculate checksum
        Serial.println(serial_out);
      }  

      if (serial_in[3]=='R' & serial_in[4]=='S' & serial_in[5]==' ')  // Read State command?
      {
        if (heater_state == HIGH)
          sprintf(serial_out, "00:Output 1-ON"); // <- todo calculate checksum
        else
          sprintf(serial_out, "00:Output 1-OFF"); // <- todo calculate checksum
        Serial.println(serial_out);
      }  

      if (serial_in[3]=='R' & serial_in[4]=='T'& serial_in[5]==' ')  // Read Time command?
      {
        sprintf(serial_out, "00:%s %02d/%02d/%04d %02d:%02d:%02d ", 
        dow[time.day - 1], time.date , time.mon, time.yr,
        time.hr, time.min, time.sec );
        Serial.println(serial_out);
      }      

      if (serial_in[3]=='R' & serial_in[4]=='P' & serial_in[5]==' ')  // Read Program command?
      {
        sprintf(serial_out, "00:");
        Serial.print(serial_out);

        for (int i=0; i<=num_progs; i++)
        {

          if (progs[i].setpoint>=0)
            sprintf(serial_out, "%02d-%03d%02d%02d+%02d,", 
            i, progs[i].days, progs[i].hour, progs[i].minute, progs[i].setpoint );
          else
            sprintf(serial_out, "%02d-%03d%02d%02d-%02d,", 
            i, progs[i].days, progs[i].hour, progs[i].minute, progs[i].setpoint );

          Serial.print(serial_out);
        }          
        Serial.println(" ");
      }
      
      
      // 
      //  Test for and Process modify commands
      //
      if (serial_in[3]=='S' & serial_in[4]=='C' & serial_in[5]==' ')  // Set Temperature command?
      {
        char Tempstr[2];
        Tempstr[0] = serial_in[6]; 
        Tempstr[1]=serial_in[7]; 
        Tempstr[2]='\0';
        int NewTemp = atoi(Tempstr);
        if ((NewTemp<40) && (NewTemp>10))
          setpoint=NewTemp;
      }
     
      if (serial_in[3]=='S' & serial_in[4]=='O' & serial_in[5]==' ' & serial_in[8]=='-')  // Set State command?
      {
        if ((serial_in[6]=='0') & (serial_in[7]=='1'))  // have they selected our only output? 
           {
             if (serial_in[9]=='1') {
                 heater_state = HIGH;
                 Serial.println("00:OK");
             }
             else if (serial_in[9]=='0') {
                 heater_state = LOW;
                  Serial.println("00:OK");
                 }
             
           }
           else Serial.println("00:NO Output ");
      }  

/*
 if (serial_in[3]=='S' & serial_in[4]=='T' & serial_in[5]==' ')  // Set Time command?
      { 
        char Tempstr[2];
        int DOW = serial_in[6]; 
        Tempstr[0] = serial_in[8]; 
        Tempstr[1] = serial_in[9]; 
        Tempstr[2]='\0';
        int DD = atoi(Tempstr);
        Tempstr[0] = serial_in[11]; 
        Tempstr[1] = serial_in[12]; 
        Tempstr[2]='\0';
        int MO = atoi(Tempstr);
        Tempstr[0] = serial_in[14]; 
        Tempstr[1] = serial_in[15]; 
        Tempstr[2]='\0';
        int YY = atoi(Tempstr);

        Tempstr[0] = serial_in[17]; 
        Tempstr[1] = serial_in[18]; 
        Tempstr[2]='\0';
        int HH = atoi(Tempstr);
        Tempstr[0] = serial_in[20]; 
        Tempstr[1] = serial_in[21]; 
        Tempstr[2]='\0';
        int MN = atoi(Tempstr);
        Tempstr[0] = serial_in[23]; 
        Tempstr[1] = serial_in[24]; 
        Tempstr[2]='\0';
        int SS = atoi(Tempstr);

        // should do range checking here...
        rtc.day(DD); rtc.month(MO); rtc.year(YY);
        rtc.hour(2000+HH); rtc.minutes(MN); rtc.seconds(SS);
        Serial.println("00:OK ");
        
       
      }
*/



    }

    // test for and process broadcast messages   
    if (( atoi(MSG_ID) == 99 ) & (serial_in[2]==':'))
    { 
      //Serial.println("Broadcast ID Matched");

      // 
      //  Test for and Process read commands
      //
      if (serial_in[3]=='I' & serial_in[4]=='D' & serial_in[5]==' ')  // ID Command?
      {
        delay(slave_id * 20); // wait the appropriate delay period based on out device ID
        sprintf(serial_out, "00:%02d OK %02d", (int) slave_id, 0); // <- todo calculate checksum
        Serial.println(serial_out);
      }
    } 


    //        serial_in[serial_read]='\0'; // add an end of string char to play nicely
    //        Serial.println(serial_in);
    serial_in[0]='\0'; // and erase the string
    serial_read=0;
  }

  // Determine button state
  // returns 1 for button1, 2 for button2, 4 for button 3, and 8 for button4
  buttons = 0;
  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (justpressed[i]) {
      justpressed[i] = 0;
      buttons |= 1<<i;
    }
  }

  //  if(buttons > 0)
  //  {
  //    snprintf(line_buffer, 100, "** Button events: %d", buttons);
  //    Serial.println(line_buffer);
  //  } // else  snprintf(line_buffer, 100, "** NO Button events: %d", buttons);


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
  case 0: 
    sprintf(menu, "Off  -   +   Set"); 
    break;
  case 1: 
    sprintf(menu, " On  -   +   Set"); 
    break;
  case 2: 
    sprintf(menu, "Time    Prog  OK"); 
    break;
  case 3: 
    sprintf(menu, "Day  -   +    OK"); 
    break;
  case 4: 
    sprintf(menu, "Hour -   +    OK"); 
    break;
  case 5: 
    sprintf(menu, "Mins -   +    OK"); 
    break;
  }  

  // Determine the time
  time = rtc.time();
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
    //snprintf(line_buffer, 100,
    //         "** Program %d matches (%d vs %d) -- setpoint %d\n",
    //         i, progs[current_prog].hour, progs[current_prog].minute,
    //         setpoint);
    //Serial.println(line_buffer);
    prev_prog = current_prog;
  }

  // Determine the temperature
  if(millis() - last_temp > 5000 || temp < -200.0)
  {
    //Serial.println("** Read temperature\n");
    sensors.requestTemperatures();
    while(sensors.getDeviceCount() == 0)
    {
      sensors.begin();
    }

    temp = 0.0;
    for(i = 0; i < sensors.getDeviceCount(); i++)
      temp += sensors.getTempCByIndex(i);
    temp /= sensors.getDeviceCount();

    //Serial.println(temp);
    //snprintf(line_buffer, 100, "Inside heater controller: %s\n",
    //         ftoa(float_conv, temp, 2);
    //Serial.println(line_buffer);

    last_temp = millis();
  }

  // Check the temperature
  if(temp < setpoint && setpoint - temp > 0.25)
  {
    if(heater_state != HIGH){
      //Serial.println("** Heater on\n");
      heater_on = millis();
    }
    heater_state = HIGH;
  }
  else if(millis() - heater_on > 300000)
  {
    if(heater_state != LOW){
      //Serial.println("** Heater off\n");
      //snprintf(line_buffer, 100, "Heater run time: %ul\n",
      //         (millis() - heater_on));
      //Serial.println(line_buffer);
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
    //  Serial.println(">> ");
    //  Serial.println(data);
    //  Serial.println("\n");
    lcd.setCursor(0, 0);
    lcd.print(data);
    strcpy(prev_data, data);

    //  Serial.println(">> ");
    //  Serial.println(menu);
    //  Serial.println("\n");
    lcd.setCursor(0, 1);
    lcd.print(menu);
    strcpy(prev_menu, menu);
  }

  // Poor man's debounce
  //if(buttons > 0) delay(100);
}
