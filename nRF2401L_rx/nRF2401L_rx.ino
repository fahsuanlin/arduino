// nrf24_rx_verbose.ino
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN  9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);

const byte PIPE_ADDR[6] = "NODE1";

unsigned long lastStatus = 0;

void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println(F("RX verbose boot"));

  if (!radio.begin()) {
    Serial.println(F("ERROR: radio.begin() failed — wiring/power"));
    while (1) { delay(1000); }
  }

  radio.setAutoAck(false);            // must match TX
  radio.setChannel(76);               // must match TX
  radio.setDataRate(RF24_1MBPS);      // must match TX
  radio.setPALevel(RF24_PA_MIN);

  radio.disableDynamicPayloads();
  radio.setPayloadSize(5);

  radio.openReadingPipe(0, PIPE_ADDR);
  radio.startListening();

  Serial.println(F("RX ready"));
  // Optional: radio.printDetails(); // uncomment after it works
}

void loop() {
  // Periodically print basic RF status (energy detect)
  if (millis() - lastStatus > 1000) {
    lastStatus = millis();

    // Quick energy check: testRPD() returns true if > -64 dBm since last call
    bool energy = radio.testRPD();
    Serial.print(F("Status: RPD="));
    Serial.print(energy ? F("HIGH") : F("LOW"));
    Serial.print(F("  CH=")); Serial.print(76);
    Serial.print(F("  DR=1Mbps  PA=MIN"));
    Serial.println();
  }

  if (!radio.available()) return;

  char buf[6];              // 5 + '\0'
  radio.read(buf, 5);       // read exactly 5 bytes
  buf[4] = '\0';            // ensure printable

  Serial.print(F("RX raw: "));
  for (uint8_t i = 0; i < 5; ++i) {
    Serial.print((uint8_t)buf[i], HEX); Serial.print(' ');
  }
  Serial.print(F("  text:[")); Serial.print(buf); Serial.println(F("]"));
}
