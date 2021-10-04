int ttl_port = 3;
int pass_port = 5; 

int time_ttl; //time instant for input

int debounce_time_ttl = 500; //debounce time; ms

int led_status = 0;

int time_input; //time instant for input

int debounce_time_input= 500; //debounce time; ms


void setup() {

  Serial.begin(9600);
  pinMode(ttl_port, OUTPUT);
  pinMode(pass_port, OUTPUT);

  time_input = millis();
  time_ttl=0;
}

void loop() {

   if(millis()-time_input>debounce_time_input){
        digitalWrite(pass_port, LOW);   // turn the LED off
    }

   if(millis()-time_ttl>debounce_time_ttl){
        digitalWrite(ttl_port, LOW);   // turn the LED off
    }
    
  if (Serial.available()) {      // If anything comes in Serial (USB),
    //Serial.println("INPUT");
    int incomingByte = Serial.read();

    //receive input?
    if (incomingByte>0) {
      digitalWrite(pass_port, HIGH);   // turn the LED on (HIGH is the voltage level)
      time_input = millis();     
    }

    //receive trigger?
    if (incomingByte>48) { //"0": off; "1": on; ASCII code
       if(millis()-time_ttl>debounce_time_ttl){
          digitalWrite(ttl_port, HIGH);   // turn the LED on (HIGH is the voltage level)
          time_ttl=millis();
       }
      led_status = 1;
    }
    else {
    //Serial.println("NOTHING");
      if(led_status>0){
         if(millis()-time_ttl>debounce_time_ttl){
          digitalWrite(ttl_port, LOW);
          led_status = 0;
         }
      }
    }
  }
}
