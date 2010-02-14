#include <MsTimer2.h>

#define LATCHPIN 10
#define CLOCKPIN 12
#define DATAPIN  11

int powers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
byte red[8], green[8];

void cleardisplay() {
  int i;
  for(i = 0; i < 8; i++) {
    red[i] = 0;
    green[i] = 0;
  }
}

void printbinary(byte d) {
  int i;
  for(i = 0; i < 8; i++) {
    if(d & powers[i]) Serial.print("1");
    else Serial.print("0");
  }
}

void setpixel(int x, int y, byte *color) {
  if(!(color[y] & powers[x]))
    color[y] += powers[x];
}

// Flush the display state to the shift registers. This is done in an ISR and can't
// use delay(), hence the slightly odd looping.
void writedisplay() {
  static int i;
  
  i++;
  if(i == 8) i = 0;

  if(green[i] != 0 || red[i] != 0) {
    digitalWrite(LATCHPIN, LOW);
    shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, powers[i]);
    shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, green[i]);
    shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, red[i]);
    digitalWrite(LATCHPIN, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  
  cleardisplay();
  MsTimer2::set(2, writedisplay);
  MsTimer2::start();
}

void loop() {
  int x, y;
  
  for(x = 0; x < 8; x++) {
    cleardisplay();
    setpixel(x, 1, red);
    delay(100);
  }
}
