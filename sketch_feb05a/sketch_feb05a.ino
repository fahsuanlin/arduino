
int led = 13;

void setup() {
  // put your setup code here, to run once:
pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(led, HIGH); 
delay(1000); 
digitalWrite(led, LOW); 
delay(100);
// turn the LED on (HIGH is the voltage level) // wait for a second
// turn the LED off by making the voltage LOW // wait for a second
}
