int led = 13;
int led_status = 0;

void setup() {

  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {

  if (Serial.available()) {      // If anything comes in Serial (USB),
    //Serial.println("INPUT");
    int incomingByte = Serial.read();
    if (incomingByte>47) { //"0": off; "1": on; ASCII code
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      led_status = 1;
      delay(200); //debounce
    }
    else {
    //Serial.println("NOTHING");
      if(led_status>0){
        digitalWrite(led, LOW);
        led_status = 0;
      }
    }
  }
}
