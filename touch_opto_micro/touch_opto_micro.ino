#include <U8g2lib.h>
#include <U8x8lib.h>

#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

#define BUTTON_PIN A1 //arduino micro D4
#define PHOTO_PIN A0  //arduino micro A0
#define PHOTO_LED_PIN A8 //arduino micro A1
#define BUTTON_LED_PIN A9 //arduino mciro A2

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

unsigned long photo_status;
unsigned long touch_status;

String photo_status_str;
String touch_status_str;

String led_string;
char led_string_buffer[50];


struct touch {
  byte wasPressed = LOW;
  byte isPressed = LOW;
};

touch touch;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

int photo_val = 0;

void setup()
{
  u8g2.begin();

  pinMode(PHOTO_LED_PIN, OUTPUT);
  pinMode(BUTTON_LED_PIN, OUTPUT);
  pinMode(PHOTO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
  startMillis = millis();  //initial start time

  photo_status = 0; //no light
  touch_status = 0; //no touch

  photo_status_str = "-";
  touch_status_str = "-";


}
void loop()
{
  currentMillis = millis(); //get the current "time"


  led_string = photo_status_str + " " + touch_status_str;
  led_string.toCharArray(led_string_buffer, 50);

  //Serial.println(currentMillis - startMillis);

  touch.isPressed = isTouchPressed(BUTTON_PIN);
  if (touch.wasPressed != touch.isPressed) {
    if (touch.isPressed) {
      //Serial.println("Touch pressed");
      touch_status = 1;
      touch_status_str = "T";
      led_string = photo_status_str + " " + touch_status_str;
      led_string.toCharArray(led_string_buffer, 50);
      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
      u8g2.drawStr(8, 29, led_string_buffer);
      u8g2.sendBuffer();         // transfer internal memory to the display


      float lapsedtime = currentMillis - startMillis;
      Serial.print(led_string);
      Serial.print(": ");
      Serial.print(lapsedtime / 1e3, 3);
      Serial.print(", ");
      Serial.println(photo_status);
      Serial.println();

      analogWrite(BUTTON_LED_PIN, 255);
      //delay(1);
    }
  }
  else {
    analogWrite(BUTTON_LED_PIN, 0);
    touch_status = 0;
    touch_status_str = "-";
    led_string = photo_status_str + " " + touch_status_str;
    led_string.toCharArray(led_string_buffer, 50);
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
    u8g2.drawStr(8, 29, led_string_buffer);
    u8g2.sendBuffer();         // transfer internal memory to the display
  }

  touch.wasPressed = touch.isPressed;

  photo_val = digitalRead(PHOTO_PIN);

  //Serial.println("Min:1000,Max:1023");
  //delay(1);

  if (photo_val == LOW) {
    photo_status_str = "L";
    led_string = photo_status_str + " " + touch_status_str;
    led_string.toCharArray(led_string_buffer, 50);
    if (photo_status == 0) {

      analogWrite(PHOTO_LED_PIN, 255);

      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
      //u8g2.drawStr(8, 29, "On"); // write something to the internal memory
      u8g2.drawStr(8, 29, led_string_buffer);

      u8g2.sendBuffer();         // transfer internal memory to the display

      photo_status = 1;

      float lapsedtime = currentMillis - startMillis;
      Serial.print(led_string);
      Serial.print(": ");
      Serial.print(lapsedtime / 1e3, 3);
      Serial.print(", ");
      Serial.println(photo_status);
      Serial.println();
    }
  }
  else {
    photo_status_str = "-";
    led_string = photo_status_str + " " + touch_status_str;
    led_string.toCharArray(led_string_buffer, 50);
    if (photo_status == 1) {

      analogWrite(PHOTO_LED_PIN, 0);

      u8g2.clearBuffer();          // clear the internal memory
      u8g2.setFont(u8g2_font_logisoso28_tr);  // choose a suitable font at https://github.com/olikraus/u8g2/wiki/fntlistall
      //u8g2.drawStr(8, 29, "Off"); // write something to the internal memory
      u8g2.drawStr(8, 29, led_string_buffer);
      u8g2.sendBuffer();         // transfer internal memory to the display

      photo_status = 0;


      float lapsedtime = currentMillis - startMillis;
      Serial.print(led_string);
      Serial.print(": ");
      Serial.print(lapsedtime / 1e3, 3);
      Serial.print(", ");
      Serial.println(photo_status);
      Serial.println();

    }
  }
}

bool isTouchPressed(int pin)
{
  return digitalRead(pin) == HIGH;
}
