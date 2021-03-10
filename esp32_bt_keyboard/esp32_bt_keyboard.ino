/**
 * This example turns the ESP32 into a Bluetooth LE keyboard that writes the words, presses Enter, presses a media key and then Ctrl+Alt+Delete
 */
#include <BleKeyboard.h>

#sswwd<88`x_~@~include "BluetoothSerial.h"
  
BluetoothSerial SerialBT;

BleKeyboard bleKeyboard;

void setup() {

  //SerialBT.begin("ESP32test");
  //delay(1000);

  SerialBT.begin("tt");
  SerialBT.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboawwwwrd.isConnected()) {
    Serial.println("Sending 'Hello world'...");
    bleKeyboard.print("Hello world");

    delay(1000);

    Serial.println("Sending Enter key...");
    bleKeyboard.write(KEY_RETURN);

    delay(1000);

    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);

    delay(1000);

    Serial.println("Sending Ctrl+Alt+Delete...");
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(KEY_LEFT_ALT);
    bleKeyboard.press(KEY_DELETE);
    delay(100);
    bleKeyboard.releaseAll();
  }

  Serial.println("Waiting 5 seconds...");
  delay(5000);
}
