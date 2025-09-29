int HallSensor = 19; //pin "A1" Hall sensor is connected to the D2 pin
int LED = 18; // pin "A0"
int LED1 = 20; // pin "A2"
byte brightness = 255;

void setup() {
  Serial.begin(9600);

  pinMode(HallSensor, INPUT); // Hall Effect Sensor pin INPUT
  pinMode(LED, OUTPUT); // LED Pin Output
}
void loop() {
  int sensorStatus = analogRead(HallSensor); // Check the sensor status
  Serial.println(sensorStatus);
  if (sensorStatus <1000) // Check if the pin high or not
  {
    // if the pin is high turn on the onboard Led
    //digitalWrite(LED, HIGH); // LED on
    digitalWrite(LED, HIGH);
    digitalWrite(LED1, HIGH);
  }
  else  {
    //else turn off the onboard LED
    //digitalWrite(LED, LOW); // LED off
    digitalWrite(LED, LOW);
    digitalWrite(LED1, LOW);
  }
}
