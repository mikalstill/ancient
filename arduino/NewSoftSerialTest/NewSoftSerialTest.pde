#include <NewSoftSerial.h>

// Note you can't use pin 13, as it has LED hardware which gets in the way
NewSoftSerial mySerial(10, 11, true);

void setup()  
{
  // Serial speeds need to match of weird things happen...
  Serial.begin(9600);
  mySerial.begin(9600);
  
  Serial.println("Goodnight moon!");
  Serial.print("CPU speed: ");
  Serial.println(F_CPU);

  // set the data rate for the NewSoftSerial port
  delay(1000);
  mySerial.println("");
  mySerial.println("Hello, world?");
}

void loop()
{
  if (mySerial.available()) {
      Serial.print((char)mySerial.read());
  }
  if (Serial.available()) {
      mySerial.print((char)Serial.read());
  }
}
