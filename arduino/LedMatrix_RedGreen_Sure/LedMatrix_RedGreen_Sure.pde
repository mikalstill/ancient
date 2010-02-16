// This code is for the Sure LED matrix from Sparkfun. It has a rather
// non-sensible pinout for the matrix. There are lots of software fixups to try
// and make the hardware a little bit easier to wire here. See the San Young
// version of this code for a more sensible implementation.

#include <MsTimer2.h>

#define LATCHPIN 10
#define CLOCKPIN 12
#define DATAPIN  11

byte powers[] = {1, 2, 4, 8, 16, 32, 64, 128};
byte red[8], green[8];

void cleardisplay() {
  int i;
  for(i = 0; i < 8; i++) {
    red[i] = 255;
    green[i] = 255;
  }
}

void setpixel(int x, int y, byte *color) {
  if(!(color[y] & powers[x]))
    color[y] += powers[x];
}

// Flush the display state to the shift registers. This is done in an ISR and
// can't use delay(), hence the slightly odd looping.
void writedisplay() {
  static int i = 0;
  
  i++;
  if(i == 8) i = 0;

  digitalWrite(LATCHPIN, LOW);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, 0);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, 0);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, 255 - powers[i]);
  digitalWrite(LATCHPIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  
  cleardisplay();
  randomSeed(analogRead(0));
  MsTimer2::set(1000, writedisplay);
  MsTimer2::start();
}

void loop() {
}
