// Serial sniffer for an Arlec PC222 power meter

#define CLOCKPIN 2
#define CSPIN    3
#define SPIPIN   4
#define SPOPIN   5

void setup() {
  Serial.begin(115200);
  
  pinMode(CSPIN, INPUT);
  pinMode(SPIPIN, INPUT);
  pinMode(SPOPIN, INPUT);
  attachInterrupt(0, interrupt, RISING);
}

void loop() {
  
}

void interrupt() {
  // If CS is low, recognize CLK
  if(digitalRead(CSPIN) == LOW)
  {
    Serial.print(digitalRead(SPIPIN) == HIGH ? "1" : "0"));
    Serial.println(digitalRead(SPOPIN) == HIGH ? "1" : "0"));
  }
}
