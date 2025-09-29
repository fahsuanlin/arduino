// nrf24_tx_simple.ino
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);

const byte PIPE_ADDR[6] = "NODE1";  // 5 bytes + '\0'

void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println(F("TX simple boot"));

  if (!radio.begin()) {
    Serial.println(F("ERROR: radio.begin() failed — wiring/power"));
    while (1) { delay(1000); }
  }

  radio.setAutoAck(false);            // turn off ACKs to simplify
  radio.setChannel(76);               // change later if needed (0–125)
  radio.setDataRate(RF24_1MBPS);      // try RF24_250KBPS if clones are finicky
  radio.setPALevel(RF24_PA_MIN);      // low power for bench distance

  radio.disableDynamicPayloads();     // fixed size
  radio.setPayloadSize(5);            // we'll send "TEST\0"

  radio.openWritingPipe(PIPE_ADDR);
  radio.stopListening();

  Serial.println(F("TX ready"));
}

void loop() {
  char msg[5] = {'T','E','S','T','\0'}; // 5 bytes
  bool ok = radio.write(msg, 5);
  Serial.print(F("TX sent 'TEST' ok="));
  Serial.println(ok ? F("yes") : F("no"));
  delay(200);
}
