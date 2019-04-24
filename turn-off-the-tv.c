#include <IRremote.h>
 
int pirPin = 7;
IRsend irsend; // 3
int ledPin = 6;
 
const int timeLimit = 300; // How many seconds before the OFF signal is sent
int currentCount = timeLimit; // The current counter, start it at max and count down
int warningThreshold = 60; // How many seconds before the warning LED flashes
 
int brightness = 0; // How bright the Warning LED is
int fadeAmount = 5; // How many points to fade the Warning LED by each step
int fadeSpeed = 10; // How much delay in the fade
 
// 0x20DF10EF is ON/OFF for LG TVs
#define IR_CODE 0x20DF10EF
 
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, LOW); // Turn the Warning LED off to start with
}
 
void loop() {
  Serial.println(currentCount);
  currentCount = currentCount - 1;
 
  // If the warning threshold has been reached, flash the warning LED!
  if (currentCount < warningThreshold) {
    for (brightness = 0; brightness < 255; brightness = brightness + fadeAmount ) {
      analogWrite(ledPin, brightness);
      delay(fadeSpeed);
    }
    for (brightness = 255; brightness > 0;  brightness = brightness - fadeAmount ) {
      analogWrite(ledPin, brightness);
      delay(fadeSpeed);
    }
  } else {
    digitalWrite(ledPin, LOW);
    delay(1000);
  }
 
  // If we detect movement, reset the count and start again
  if (digitalRead(pirPin) == true) {
    currentCount = timeLimit;
    Serial.println("Movement Detected");
  }
 
  // If the counter hits zero, turn off the TV!
  if (currentCount == 0) {
    digitalWrite(ledPin, LOW);
    Serial.print("Sending OFF now... ");
    delay(200);
    irsend.sendNEC(IR_CODE, 32); // SEND "TV OFF"
    // From here on probably won't happen, but it's there for testing...
    Serial.println("Done.");
    currentCount = timeLimit;
  }
}
