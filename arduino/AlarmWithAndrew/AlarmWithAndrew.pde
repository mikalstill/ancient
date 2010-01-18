// Keypad example from http://www.arduino.cc/playground/Main/KeypadTutorial

#include <Keypad.h>

#define ROWS 4
#define COLS 4

char keys[ROWS][COLS] = {
  {'1', '4', '7', '*'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};

// This is arduino pins not keypad pins!
byte rowPins[ROWS] = { 3, 4, 5, 6 };
byte colPins[COLS] = { 7, 8, 9, 10 }; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define SENSORPIN 2
#define ENTERLED 13
#define RIGHTLED 12
#define WRONGLED 11
boolean codeOk = true;

char entered[4];
unsigned long lastEvent = 0;

void enter()
{
  int i;
  
  digitalWrite(ENTERLED, HIGH);
  digitalWrite(RIGHTLED, LOW);
  digitalWrite(WRONGLED, LOW);
  codeOk = false;
  
  for(i = 0; i < 4; i++)
    entered[i] = '\0';
}

void drawopen(void)
{
  if(!codeOk)
  {
    digitalWrite(ENTERLED, LOW);
    digitalWrite(WRONGLED, HIGH);
  }
}

void setup()
{
  pinMode(ENTERLED, OUTPUT);
  pinMode(RIGHTLED, OUTPUT);
  pinMode(WRONGLED, OUTPUT);
  
  attachInterrupt(0, drawopen, FALLING);
  
  lastEvent = millis();
  enter();
  Serial.begin(9600);
}

void loop()
{
  char key = kpd.getKey();
  int i;

  if(millis() - lastEvent > 60000)
    enter();

  if(key)  // same as if(key != NO_KEY)
  {
    lastEvent = millis();
    switch (key)
    {
      case '*':
        enter();
        break;
        
      case '#':
        digitalWrite(WRONGLED, HIGH);
        digitalWrite(ENTERLED, LOW);
        break;
        
      default:
        Serial.println(key);
        
        for(i = 0; i < 4; i++)
          if(entered[i] == '\0')
            Serial.print('-');
          else
            Serial.print(entered[i]);

        for(i = 0; i < 4; i++)
        {
          if(entered[i] == '\0')
          {
            entered[i] = key;
            break;
          }
        }
        
        Serial.print(' ');
        Serial.println(i);
        
        if(i == 3)
        {
          digitalWrite(ENTERLED, LOW);
          if(entered[0] == '1' && entered[1] == '2' && entered[2] == '3' && entered[3] == '4')
          {
            Serial.println("CORRECT CODE!");
            digitalWrite(RIGHTLED, HIGH);
            codeOk = true;
          }
          else
          {
            Serial.println("WRONG CODE!");
            digitalWrite(WRONGLED, HIGH);
          }
        }
      }
  }
}

