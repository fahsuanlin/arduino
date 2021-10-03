int ttl_port = 3;
int debounce_time = 200; //debounce time; ms

int led_status = 0;

void setup() {

  Serial.begin(9600);
  pinMode(ttl_port, OUTPUT);
}

void loop() {

  if (Serial.available()) {      // If anything comes in Serial (USB),
    //Serial.println("INPUT");
    int incomingByte = Serial.read();
    if (incomingByte>48) { //"0": off; "1": on; ASCII code
      digitalWrite(ttl_port, HIGH);   // turn the LED on (HIGH is the voltage level)
      led_status = 1;
      delay(debounce_time); //debounce
    }
    else {
    //Serial.println("NOTHING");
      if(led_status>0){
        digitalWrite(ttl_port, LOW);
        led_status = 0;
      }
    }
  }
}
