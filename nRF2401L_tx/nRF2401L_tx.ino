#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
int button_pin = 4;
boolean button_state = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Tx");
  
  pinMode(button_pin, INPUT);
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}
void loop()
{
  button_state = digitalRead(button_pin);
  if (button_state == HIGH)
  {
    Serial.println("TX:HIGH");
    const char text[] = "1111";
    //radio.write(&text, sizeof(text));                  //Sending the message to receiver
    button_state=1;
    radio.write(&button_state, sizeof(button_state));                  //Sending the message to receiver
    Serial.println(button_state);
  }
  else
  {
    Serial.println("TX:LOW");
    const char text[1] = "0000";
    //radio.write(&text, sizeof(text));                  //Sending the message to receiver
    button_state=0;
    radio.write(&button_state, sizeof(button_state));                  //Sending the message to receiver
    Serial.println(button_state);
  }
  //radio.write(&button_state, sizeof(button_state));  //Sending the message to receiver
  delay(10);
}
