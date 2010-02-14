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

void printbinary(unsigned int d) {
  int i;
  
  for(i = 0; i < 8; i++) {
    if(d & powers[i]) Serial.print("1");
    else Serial.print("0");
  }
}

void setredpixel(int x, int y) {
  if(!(redcols & powers[x]))
    redcols += powers[x];
  if(!(rows & powers[y]))
    rows += powers[y];
}

void setgreenpixel(int x, int y) {
  if(!(greencols & powers[x]))
    greencols += powers[x];
  if(!(rows & powers[y]))
    rows += powers[y];
}

// Flush the display state to the shift registers
void writedisplay(byte rc, byte gc, byte r) {
  printbinary(rc);
  Serial.print(", ");
  printbinary(gc);
  Serial.print(", ");
  printbinary(r);
  Serial.println("");
  
  digitalWrite(LATCHPIN, LOW);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, r);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, gc);
  shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, rc);
  digitalWrite(LATCHPIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
}

void loop() {
//  int x, y;
//  
//  for(y = 0; y < 8; y++) {
//    for(x = 0; x < 8; x++) {
//      cleardisplay();
//      setredpixel(x, y);
//      setgreenpixel(7 - x, 7 - y);
//      writedisplay(redcols, greencols, rows);
//      delay(30);
//    }
//  }

  cleardisplay();
  setredpixel(0, 0);
  writedisplay(redcols, greencols, rows);
  cleardisplay();
  setgreenpixel(7, 7);
  writedisplay(redcols, greencols, rows);
}
