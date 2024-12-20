/*********************************************************************
  This is an example for our nRF51822 based Bluefruit LE modules

  Pick one up today in the adafruit shop!

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  MIT license, check LICENSE for more information
  All text above, and the splash screen below must be included in
  any redistribution
*********************************************************************/

/*
  This example shows how to send HID (keyboard/mouse/etc) data via BLE
  Note that not all devices support BLE keyboard! BLE Keyboard != Bluetooth Keyboard
*/

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
#include <SoftwareSerial.h>
#endif

/*=========================================================================
    APPLICATION SETTINGS

      FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
     
                                Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                                running this at least once is a good idea.
     
                                When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                                Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
         
                                Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    -----------------------------------------------------------------------*/
#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
/*=========================================================================*/


// Create the bluefruit object, either software serial...uncomment these lines
/*
  SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

  Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}


int jPin = 9;   // pushbutton connected to digital pin 9
int kPin = 10;   // pushbutton connected to digital pin 10
int lPin = 11;   // pushbutton connected to digital pin 11
int iPin = 12;   // pushbutton connected to digital pin 12
int mPin = 13;   // pushbutton connected to digital pin 13
int dPin = 6;   // pushbutton connected to digital pin 6
int sPin = 5;   // pushbutton connected to digital pin 5
int aPin = 3;   // pushbutton connected to digital pin 3
int wPin = 2;   // pushbutton connected to digital pin 2
int xPin = 1;   // pushbutton connected to digital pin 1
int pressed = 0;

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************JJ************************************************/
void setup(void)
{
  //ble.reset();

  //while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit HID Keyboard Example"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'Bluefruit Keyboard': "));
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=TrueImage 10-button Kb" )) ) {
    error(F("Could not set device name?"));
  }

  /* Enable HID Service */
  Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
      error(F("Could not enable Keyboard"));
    }
  } else
  {
    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
      error(F("Could not enable Keyboard"));
    }
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }

  Serial.println();
  Serial.println(F("Go to your phone's Bluetooth settings to pair your device"));
  Serial.println(F("then open an application that accepts keyboard input"));

  Serial.println();
  Serial.println(F("Enter the character(s) to send:"));
  Serial.println(F("- \\r for Enter"));
  Serial.println(F("- \\n for newline"));
  Serial.println(F("- \\t for tab"));
  Serial.println(F("- \\b for backspace"));

  Serial.println();


  pinMode(jPin, INPUT);
  pinMode(kPin, INPUT);
  pinMode(lPin, INPUT);
  pinMode(iPin, INPUT);
  pinMode(mPin, INPUT);
  pinMode(dPin, INPUT);
  pinMode(sPin, INPUT);
  pinMode(aPin, INPUT);
  pinMode(wPin, INPUT);
  pinMode(xPin, INPUT);
   
  pinMode(LED_BUILTIN, OUTPUT);


}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  // Display prompt
  // Serial.print(F("keyboard > "));


  //int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  //float voltage = sensorValue * (5.0 / 1023.0);
  //voltage = sensorValue;
  int j_voltage = digitalRead(jPin);
  int k_voltage = digitalRead(kPin);
  int l_voltage = digitalRead(lPin);
  int i_voltage = digitalRead(iPin);
  int m_voltage = digitalRead(mPin);
  int d_voltage = digitalRead(dPin);
  int s_voltage = digitalRead(sPin);
  int a_voltage = digitalRead(aPin);
  int w_voltage = digitalRead(wPin);
  int x_voltage = digitalRead(xPin);
  // print out the value you read:
  //Serial.print("j_voltage=");
  //Serial.println(j_voltage);
  //Serial.print("k_voltage=");
  //Serial.println(k_voltage);
  //Serial.print("l_voltage=");
  //Serial.println(l_voltage);
  if (j_voltage == HIGH) {
   Serial.println("J PRESSED!");
   ble.print("AT+BleKeyboard=");
   ble.println("6");
   pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  if (k_voltage == HIGH) {
    Serial.println("K PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("7");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  if (l_voltage == HIGH) {
    Serial.println("L PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("8");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  if (i_voltage == HIGH) {
    Serial.println("I PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("9");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  if (m_voltage == HIGH) {
    Serial.println("M PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("0");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }  



  if (d_voltage == HIGH) {
    Serial.println("D PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("5");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  if (s_voltage == HIGH) {
    Serial.println("S PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("4");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  if (a_voltage == HIGH) {
    Serial.println("A PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("3");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  if (w_voltage == HIGH) {
    Serial.println("W PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("2");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  if (x_voltage == HIGH) {
    Serial.println("X PRESSED!");
    ble.print("AT+BleKeyboard=");
    ble.println("1");
    pressed = 1;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }  
  
  if (pressed) {
    delay(150);
    j_voltage = digitalRead(jPin);
    k_voltage = digitalRead(kPin);
    l_voltage = digitalRead(lPin);
    i_voltage = digitalRead(iPin);
    m_voltage = digitalRead(mPin);
    d_voltage = digitalRead(dPin);
    s_voltage = digitalRead(sPin);
    a_voltage = digitalRead(aPin);
    w_voltage = digitalRead(wPin);
    x_voltage = digitalRead(xPin);
//    if (j_voltage == LOW && k_voltage == LOW && l_voltage == LOW && i_voltage == LOW && m_voltage == LOW &&d_voltage == LOW && s_voltage == LOW && a_voltage == LOW && w_voltage == LOW && x_voltage == LOW) {
      pressed = 0;
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//    }
  }

  /*
    // Check for user input and echo it back if anything was found
    char keys[BUFSIZE+1];
    getUserInput(keys, BUFSIZE);

    Serial.print("\Getting ");
    Serial.println(keys);
    delay(1000);
    (*keys)++;
    Serial.print("\nSending ");
    Serial.println(keys);

    ble.print("AT+BleKeyboard=");
    ble.println(keys);

    if( ble.waitForOK() )
    {
    Serial.println( F("OK!") );
    }else
    {
    Serial.println( F("FAILED!") );
    }
  */
}

/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
*/
/**************************************************************************/
void getUserInput(char buffer[], uint8_t maxSize)
{
  memset(buffer, 0, maxSize);
  while ( Serial.available() == 0 ) {
    delay(1);
  }

  uint8_t count = 0;

  do
  {
    count += Serial.readBytes(buffer + count, maxSize);
    delay(2);
  } while ( (count < maxSize) && !(Serial.available() == 0) );
}
