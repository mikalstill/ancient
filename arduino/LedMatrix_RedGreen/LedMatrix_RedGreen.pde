#define LATCHPIN 10
#define CLOCKPIN 12
#define DATAPIN  11

int powers[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
unsigned int redcols, greencols, rows;

void cleardisplay() {
  redcols = 0;
  greencols = 0;
  rows = 0;
}

void setredpixel(int x, int y) {
  redcols += powers[x];
  rows += powers[y];
}

void setgreenpixel(int x, int y) {
  greencols += powers[x];
  rows += powers[y];
}

// Flush the display state to the shift registers
void writedisplay(byte rc, byte gc, byte r) {
  digitalWrite(LATCHPIN, LOW);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, r);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, gc);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, rc);
  delay(200);
  digitalWrite(LATCHPIN, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  
  cleardisplay();
}

void loop() {
  int x, y;
  
  for(y = 0; y < 8; y++) {
    for(x = 0; x < 8; x++) {
      cleardisplay();
      setredpixel(x, y);
      writedisplay(redcols, greencols, rows);
      delay(30);
    }
  }
  
  for(y = 0; y < 8; y++) {
    for(x = 0; x < 8; x++) {
      cleardisplay();
      setgreenpixel(x, y);
      writedisplay(redcols, greencols, rows);
      delay(30);
    }
  }
}
