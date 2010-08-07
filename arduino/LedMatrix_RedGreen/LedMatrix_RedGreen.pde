// This code is for the San Young LED matrix from Sparkfun. It has a sensible
// pinout for the matrix.

#include <MsTimer2.h>

#define LATCHPIN 10
#define CLOCKPIN 12
#define DATAPIN  11

byte powers[] = {1, 2, 4, 8, 16, 32, 64, 128};
byte red[8], green[8];

void cleardisplay() {
  int i;
  for(i = 0; i < 8; i++) {
    red[i] = 0;
    green[i] = 0;
  }
}

void reddisplay() {
  int i;
  for(i = 0; i < 8; i++) {
    red[i] = 255;
  }
}

void greendisplay() {
  int i;
  for(i = 0; i < 8; i++) {
    green[i] = 255;
  }
}

void setpixel(int x, int y, byte *color) {
  if(!(color[y] & powers[x]))
    color[y] += powers[x];
}

void clearpixel(int x, int y, byte *color) {
  if(color[y] & powers[x])
    color[y] -= powers[x];
}

// Flush the display state to the shift registers. This is done in an ISR and
// can't use delay(), hence the slightly odd looping.
void writedisplay() {
  static int i = 0;
  
  i++;
  if(i == 8) i = 0;

  digitalWrite(LATCHPIN, LOW);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, powers[i]);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, green[i]);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, red[i]);
  digitalWrite(LATCHPIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  
  cleardisplay();
  reddisplay();
  greendisplay();
  randomSeed(analogRead(0));
  MsTimer2::set(2, writedisplay);
  MsTimer2::start();
  
  delay(1000);
  cleardisplay();
}

void loop() {
  if(random(0, 10) == 3) cleardisplay();
  setpixel(random(0, 8), random(0, 8), red);
  setpixel(random(0, 8), random(0, 8), green);
  delay(100);
}
