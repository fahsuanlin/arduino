char trigger_char = '1'; //trigger for TTL pulse
char end_char = '9'; //end-of-experiment

int ttl_port = 3; //port for sending TTL pulse
int pass_port = 5;  //port for forwarding input; D5
int pass_port_dual = 4;  //port for forwarding input; D4

unsigned long time_ttl; //time instant for input
unsigned long time_ttl_buffer[200]; //time instant for input
int debounce_time_ttl = 100; //debounce time; ms
unsigned long n_ttl;

int led_status = 0;

unsigned long time_input; //time instant for input
int debounce_time_input = 100; //debounce time; ms
unsigned long n_input;

byte buf[4];

void setup() {

  Serial.begin(57600);
  pinMode(ttl_port, OUTPUT);
  pinMode(pass_port, OUTPUT);
  pinMode(pass_port_dual, OUTPUT);

  time_input = millis();
  n_input = 0;

  time_ttl = 0;
  n_ttl = 0;
}

void loop() {

  if (millis() - time_input > debounce_time_input) {
    digitalWrite(pass_port, LOW);   // turn the input LED off
  }

 if (millis() - time_input > debounce_time_input) {
    digitalWrite(pass_port_dual, LOW);   // turn the input LED off
  }
  
  if (millis() - time_ttl > debounce_time_ttl) {
    digitalWrite(ttl_port, LOW);   // turn the TTL LED off
  }

  if (Serial.available()) {      // If anything comes in Serial (USB),
    //Serial.println("INPUT");
    int incomingByte = Serial.read();


    //receive input?
    if (incomingByte > 0 && incomingByte != end_char) {
      digitalWrite(pass_port, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(pass_port_dual, HIGH);   // turn the LED on (HIGH is the voltage level)
      Serial.println("key!!");

      n_input++;

      if (millis() - time_input > debounce_time_input) {

        time_input = millis();

        buf[0] = time_input & 255;
        buf[1] = (time_input >> 8)  & 255;
        buf[2] = (time_input >> 16) & 255;
        buf[3] = (time_input >> 24) & 255;

        Serial.write(buf, sizeof(buf));
      }
    }

    //receive trigger?
    if (incomingByte == trigger_char) {  //"1" as the trigger character
      digitalWrite(ttl_port, HIGH);   // turn the LED on (HIGH is the voltage level)
      Serial.println("trigger 1");
      
      if (millis() - time_ttl > debounce_time_ttl) {
        time_ttl = millis();
        time_ttl_buffer[n_ttl] = time_ttl;
        n_ttl++;
      }
      led_status = 1;
    }
    else if (incomingByte == end_char) { //"9" as the end-of-experiment character; send total number of TTL pulses and their timing stamps
      Serial.println("9");

      buf[0] = n_ttl & 255;
      buf[1] = (n_ttl >> 8)  & 255;
      buf[2] = (n_ttl >> 16) & 255;
      buf[3] = (n_ttl >> 24) & 255;

      Serial.write(buf, sizeof(buf));

      for (int index = 0; index < n_ttl; index++) {
        buf[0] = time_ttl_buffer[index] & 255;
        buf[1] = (time_ttl_buffer[index] >> 8)  & 255;
        buf[2] = (time_ttl_buffer[index] >> 16) & 255;
        buf[3] = (time_ttl_buffer[index] >> 24) & 255;

        Serial.write(buf, sizeof(buf));
      }
    }
    else {
      //Serial.println("NOTHING");
      //if (led_status > 0) {
      //  if (millis() - time_ttl > debounce_time_ttl) {
      //    digitalWrite(ttl_port, LOW);
      //    led_status = 0;
      //  }
      //}
    }
  }
}
