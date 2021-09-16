/*
  ReadAnalogVoltage

  Reads an analog input on pin 0, converts it to voltage, and prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ReadAnalogVoltage
*/

#include "Keyboard.h"

int voltage_previous = 0;
unsigned long lastDebounceTime;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  Keyboard.begin();

  voltage_previous = 0;
  lastDebounceTime = millis();

}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  voltage = sensorValue;
  // print out the value you read:
  Serial.println(voltage);

  // read the pushbutton:
  // if the button state has changed,
  unsigned long debounceDelay = 100;
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime=millis();
    if (abs(voltage - voltage_previous) > 100) {
      // increment the button counter
      // type out a message
      Keyboard.print("=");
      voltage_previous = voltage;

      delay(100);
    }
    else {
      // save the current button state for comparison next time:
      //delay(100);
    }
    voltage_previous = voltage;
  }
}
