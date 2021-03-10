
char incomingBytes[10];
int vals[6];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);
  }

}

void loop() {
  if (Serial.available() > 0) {
    int ll = Serial.readBytesUntil('\n', incomingBytes, sizeof(incomingBytes) / sizeof(char)) ;
    if (ll) {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(200);
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(200);
    }
    Serial.println(ll);
  }
  else{
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  }
}
