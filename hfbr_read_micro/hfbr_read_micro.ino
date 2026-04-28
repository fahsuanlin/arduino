/*
   HFBR Fiber Receiver to TTL Output
   Input: Pin D2 (Active Low from HFBR)
   Output: Pin D3 (Active High "FLIP!" signal)
*/
#include <Arduino.h>
#include <U8g2lib.h>

#include <Keyboard.h>


String led_string;
char led_string_buffer[50];

const int fiberInputPin = 7;
const int flipOutputPin = 8; // This pin will provide the "FLIP!" signal
const int onboardLed = 13;   // Standard Nano LED
const int flipBNC = 9;    // BNC trigger

char ttl_ipput_response_char = '5';


U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

void setup() {

  u8g2.begin();

  pinMode(fiberInputPin, INPUT_PULLUP);
  pinMode(flipOutputPin, OUTPUT);
  pinMode(onboardLed, OUTPUT);
  pinMode(flipBNC, OUTPUT);

  // Ensure outputs start at 0V
  digitalWrite(flipOutputPin, LOW);
  digitalWrite(onboardLed, LOW);

  led_string = "wait...";
  led_string.toCharArray(led_string_buffer, 50);
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
  u8g2.drawStr(8, 29, led_string_buffer);
  u8g2.sendBuffer();         // transfer internal memory to the display

  Keyboard.begin();

  Serial.begin(9600);
  Serial.println("System Active: Monitoring Fiber for FLIP signal...");
}

void loop() {
  //Serial.println(analogRead(fiberInputPin));
  
  // Check if D2 is pulled LOW by the HFBR receiver
  if (digitalRead(fiberInputPin) == LOW) {

    // 1. Trigger the physical FLIP! output
    digitalWrite(flipOutputPin, HIGH);
    digitalWrite(flipBNC, HIGH);

    // 2. Trigger the onboard diagnostic LED
    digitalWrite(onboardLed, HIGH);

    // 3. Log to Serial Monitor
    Serial.println("FLIP! Pulse Generated on D3");

    led_string = "ON";
    led_string.toCharArray(led_string_buffer, 50);
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2.drawStr(8, 29, led_string_buffer);
    u8g2.sendBuffer();         // transfer internal memory to the display


    Keyboard.write(ttl_ipput_response_char); // send Keyboard stroke

    // 4. Pulse Duration (Stretched to 100ms so you can see/measure it easily)
    delay(100);

    // 5. Reset both pins to LOW
    digitalWrite(flipOutputPin, LOW);
    digitalWrite(onboardLed, LOW);
    digitalWrite(flipBNC, LOW);

    led_string = "OFF";
    led_string.toCharArray(led_string_buffer, 50);
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2.drawStr(8, 29, led_string_buffer);
    u8g2.sendBuffer();         // transfer internal memory to the display

    // 6. Lockout: Wait for the fiber to go dark again before allowing another flip
    while (digitalRead(fiberInputPin) == LOW) {
      delay(1);
    }
  }
}
