char trigger_char = '1'; //trigger for TTL pulse
char end_char = '9'; //end-of-experiment

int ttl_output = 5;  //port for forwarding input; D5
int ttl_input = 4;  //port for notifying received TTL pulse; D4
int ttl_input_monitor = 15;  //port for notifying received TTL pulse; A1  (Nano)

//int pass_port_in = 2;  //port for receiving TT: pulse; D2
//int ttl_port = 3; //port for sending TTL pulse

int ttl_output_debounce_time = 100; //debounce time; ms
int ttl_input_debounce_time = 100; //debounce time; ms


unsigned long ttl_output_time; //time instant for output
unsigned long ttl_input_time; //time instant for input

unsigned long ttl_output_n;
unsigned long ttl_input_n;

void setup() {

  Serial.begin(9600);

  pinMode(ttl_output, OUTPUT);
  pinMode(ttl_input, OUTPUT);
  pinMode(ttl_input_monitor, INPUT);

  ttl_input_time = millis();
  ttl_input_n = 0;
  digitalWrite(ttl_input, LOW);

  ttl_output_time = millis();
  ttl_output_n = 0;
  digitalWrite(ttl_output, LOW);
}

void loop() {

  if (millis() - ttl_output_time > ttl_output_debounce_time) {
    digitalWrite(ttl_output, LOW);   // turn the input LED off

    if (Serial.available()) {      // If anything comes in Serial (USB),
      int incomingByte = Serial.read();

      //receive input?
      if (incomingByte > 0 && incomingByte != end_char) {
        digitalWrite(ttl_output, HIGH);   // turn the LED on (HIGH is the voltage level)

        ttl_output_time = millis();

        ttl_output_n++;

        Serial.print("Output trigger received!! ");
        Serial.println(ttl_output_n);
     }
    }

  }

  if (millis() - ttl_input_time > ttl_input_debounce_time) {
    digitalWrite(ttl_input, LOW);   // turn the input LED off

    unsigned int incomingTTL = analogRead(ttl_input_monitor);

    if (incomingTTL > 900) {
      digitalWrite(ttl_input, HIGH);   // turn the LED on (HIGH is the voltage level)

      ttl_input_time = millis();

      ttl_input_n++;

      Serial.print("TTL input received!! ");
      Serial.println(ttl_input_n);
    }
  }
}
