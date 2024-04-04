
#include <Keyboard.h>

char trigger_char = '1'; //trigger for TTL pulse
char end_char = '9'; //end-of-experiment

int ttl_output = 5;  //port for forwarding input; D5
int ttl_input = 4;  //port for notifying received TTL pulse; D4
int ttl_input_monitor = 19;  //port for notifying received TTL pulse; A1 (Micro)
int ttl_input_bridge_monitor = 20;  //port for briding received TTL pulse; A2  (Micro)
char ttl_ipput_response_char = '5';

//int pass_port_in = 2;  //port for receiving TT: pulse; D2
//int ttl_port = 3; //port for sending TTL pulse

int ttl_output_debounce_time = 100; //debounce time; ms
int ttl_input_debounce_time = 1000; //debounce time; ms
int ttl_input_duration_time = 10; //trigger output duration; ms


unsigned long ttl_output_time; //time instant for output
unsigned long ttl_input_time; //time instant for input

unsigned long ttl_output_n;
unsigned long ttl_input_n;

void setup() {

  Serial.begin(9600);

  pinMode(ttl_output, OUTPUT);
  pinMode(ttl_input, OUTPUT);
  pinMode(ttl_input_monitor, INPUT);
  pinMode(ttl_input_bridge_monitor, INPUT);

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

  unsigned int bridge_flag = analogRead(ttl_input_bridge_monitor);
  //Serial.println(bridge_flag);
  
  if (millis() - ttl_input_time > ttl_input_duration_time) {
    digitalWrite(ttl_input, LOW);   // turn the input LED off
  }
  
  if (millis() - ttl_input_time > ttl_input_debounce_time) {
    unsigned int incomingTTL = analogRead(ttl_input_monitor);

    if (incomingTTL > 500) {
      digitalWrite(ttl_input, HIGH);   // turn the LED on (HIGH is the voltage level)

      Keyboard.press(ttl_ipput_response_char); // send Keyboard stroke

      if (bridge_flag > 500) { // bridge ttl input to output
        digitalWrite(ttl_output, HIGH);   // turn the LED on (HIGH is the voltage level)
        ttl_output_time = millis();

        ttl_output_n++;
      }
      
      ttl_input_time = millis();

      ttl_input_n++;

      Serial.print("TTL input received!! ");
      Serial.println(ttl_input_n);
    }
  }
}
