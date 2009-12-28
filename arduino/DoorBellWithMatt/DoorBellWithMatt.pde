// A simple doorbell project based on the debounce sample.
//
// You need a 10K resistor between pin 3 and ground, as well as a button between pin 3 and
// 5 volt. We also hooked up a peizo buzzer between pin 12 and ground.

#define BUTTON 2
#define SPEAKER 12
#define LED 13

// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(BUTTON);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = reading;
  }
  
  // set the LED using the state of the button:
  digitalWrite(LED, buttonState);
  digitalWrite(SPEAKER, buttonState);
  
  if(buttonState == HIGH)
  {
    for(int i = 0; i < 20; i += 2)
    {
      delay(i);
      digitalWrite(SPEAKER, LOW);
      delay(20 - i);
      digitalWrite(SPEAKER, HIGH);
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

