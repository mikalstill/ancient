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

// Flush the display state to the shift registers
void writedisplay() {
  int i;
  
  for(i = 0; i < 8; i++) {
    if(green[i] != 0 || red[i] != 0) {
      digitalWrite(LATCHPIN, LOW);
      shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, powers[i]);
      shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, green[i]);
      shiftOut(DATAPIN, CLOCKPIN, MSBFIRST, red[i]);
      digitalWrite(LATCHPIN, HIGH);
      delay(1);
    }
  }
}

unsigned char setuptimer(float frequency){
  unsigned char result;

  //Calculate the timer load value
  result=(int)((257.0-(TIMER_CLOCK_FREQ/timeoutFrequency))+0.5);
  //The 257 really should be 256 but I get better results with 257.

  //Timer2 Settings: Timer Prescaler /8, mode 0
  //Timer clock = 16MHz/8 = 2Mhz or 0.5us
  //The /8 prescale gives us a good range to work with
  //so we just hard code this for now.
  TCCR2A = 0;
  TCCR2B = 0<<CS22 | 1<<CS21 | 0<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 = 1<<TOIE2;

  //load the timer for its first cycle
  TCNT2=result;

  return(result);
}

void setup() {
  Serial.begin(115200);
  pinMode(LATCHPIN, OUTPUT);
  pinMode(CLOCKPIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT);
  
  cleardisplay();
  setpixel(0, 0, red);
  setpixel(7, 7, green);
  
  setuptimer(25);
}

void loop() {
  writedisplay();
}
