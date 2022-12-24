/*
  External EEPROM Recording & Playback Demo
  ext_eeprom_demo.ino
  Uses AT24LC256 External I2C EEPROM

  DroneBot Workshop 2019
  https://dronebotworkshop.com
*/

// Include the I2C Wire Library
#include "Wire.h"

// EEPROM I2C Address
#define EEPROM_I2C_ADDRESS 0x50
//#define EEPROM_I2C_ADDRESS 0x00

// Analog pin for potentiometer
int analogPin = 0;

// Integer to hold potentiometer value
int val = 0;

// Byte to hold data read from EEPROM
int readVal = 0;

// Integer to hold number of addresses to fill
int maxaddress = 512;

typedef struct {
  String code;
  String val;
  unsigned int code_length;
} siemens_eeprom_type;



#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 



// Function to write to EEPROOM
void writeEEPROM(int address, byte val, int i2c_address)
{
  // Begin transmission to I2C EEPROM
  Wire.beginTransmission(i2c_address);

  // Send memory address as two 8-bit bytes
  //Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB

  // Send data to be stored
  Wire.write(val);

  // End the transmission
  Wire.endTransmission();

  // Add 5ms delay for EEPROM
  delay(5);
}

// Function to read from EEPROM
byte readEEPROM(int address, int i2c_address)
{
  // Define byte for received data
  byte rcvData = 0xFF;

  // Begin transmission to I2C EEPROM
  Wire.beginTransmission(i2c_address);

  // Send memory address as two 8-bit bytes
  //Wire.write((int)(address >> 8));   // MSB
  Wire.write((int)(address & 0xFF)); // LSB

  // End the transmission
  Wire.endTransmission();

  // Request one byte of data at current memory address
  Wire.requestFrom(i2c_address, 1);

  // Read the data and assign to variable
  rcvData =  Wire.read();

  // Return the data as function output
  return rcvData;
}


void setup()
{

  String code, val;
  int address = 0;
  siemens_eeprom_type siemens_eeprom[12];

  //    'CC': 'coil_code',
  siemens_eeprom[0].code = "CC";
  siemens_eeprom[0].code_length = 2;
  siemens_eeprom[0].val = "5332";

  //    'PN': 'product_name',
  siemens_eeprom[1].code = "PN";
  siemens_eeprom[1].code_length = 2;
  siemens_eeprom[1].val = "My coil";

  //    'ID': 'siemens_part_number',
  siemens_eeprom[2].code = "ID";
  siemens_eeprom[2].code_length = 2;
  siemens_eeprom[2].val = "0";

  //    'SN': 'serial_number',
  siemens_eeprom[3].code = "SN";
  siemens_eeprom[3].code_length = 2;
  siemens_eeprom[3].val = "0000";

  //    'RV': 'revision',
  siemens_eeprom[4].code = "RV";
  siemens_eeprom[4].code_length = 2;
  siemens_eeprom[4].val = "00";

  //    'LV': 'version',
  siemens_eeprom[5].code = "LV";
  siemens_eeprom[5].code_length = 2;
  siemens_eeprom[5].val = "00";

  //    'MA': 'manufacturer',
  siemens_eeprom[6].code = "MA";
  siemens_eeprom[6].code_length = 2;
  siemens_eeprom[6].val = "fhlin";

  //    'MN': 'manufacturer_part_number',
  siemens_eeprom[7].code = "MN";
  siemens_eeprom[7].code_length = 2;
  siemens_eeprom[7].val = "0";

  //    'RS': 'regulatory_status',
  siemens_eeprom[8].code = "RS";
  siemens_eeprom[8].code_length = 2;
  siemens_eeprom[8].val = "not approved";

  //    'NI': 'num_additional_info_tags'
  siemens_eeprom[9].code = "NI";
  siemens_eeprom[9].code_length = 2;
  siemens_eeprom[9].val = "12345";

  siemens_eeprom[10].code = "IT02";
  siemens_eeprom[10].code_length = 4;
  siemens_eeprom[10].val = "";

  siemens_eeprom[11].code = "IT05";
  siemens_eeprom[11].code_length = 4;
  siemens_eeprom[11].val = "nice_coil";

  //siemens_eeprom[12].code = "EOFI";
  //siemens_eeprom[12].code_length = 4;
  //siemens_eeprom[12].val = "";


  // Connect to I2C bus as master7
  Wire.begin();

  // Setup Serial Monitor
  Serial.begin(9600);

  //-------write EEPROM --------------

  String message;
  unsigned int ll;
  String str;

//  address = 0;
//  for (int eeprom_entry_idx = 0; eeprom_entry_idx < 12; eeprom_entry_idx++) {
//
//    message = siemens_eeprom[eeprom_entry_idx].code;
//
//    for (int idx = 0; idx < message.length(); idx++) {
//      writeEEPROM(address, (byte) (message[idx]), EEPROM_I2C_ADDRESS);
//      delay(5);
//      address += 1;
//    };
//
//
//    if (message == "CC") {
//      ll = 2;
//
//      writeEEPROM(address, ll >> 8, EEPROM_I2C_ADDRESS);
//      delay(5);
//      address += 1;
//      writeEEPROM(address, ll & 0xFF , EEPROM_I2C_ADDRESS);
//      delay(5);
//      address += 1;
//
//      //read coil code
//      char ccstr[5];
//      siemens_eeprom[eeprom_entry_idx].val.toCharArray(ccstr, 5);
//      ll = strtoul(ccstr, NULL, 16);
//
//      writeEEPROM(address, ll >> 8, EEPROM_I2C_ADDRESS);
//      delay(5);
//      address += 1;
//      writeEEPROM(address, ll & 0xFF , EEPROM_I2C_ADDRESS);
//      delay(5);
//      address += 1;
//    }
//    else {
//      //write 2-byte number
//      str = siemens_eeprom[eeprom_entry_idx].val;
//      ll = str.length();
//
//      writeEEPROM(address, ll >> 8, EEPROM_I2C_ADDRESS);
//      delay(5);
//      address += 1;
//      writeEEPROM(address, ll & 0xFF , EEPROM_I2C_ADDRESS);
//      delay(5);
//      address += 1;
//
//
//      //write string
//      for (int str_idx = 0; str_idx < str.length(); str_idx++) {
//        writeEEPROM(address, str[str_idx], EEPROM_I2C_ADDRESS);
//        delay(5);
//        address += 1;
//      }
//    }
//  }
//  //write "EOFI: string
//  str = "EOFI";
//  for (int str_idx = 0; str_idx < str.length(); str_idx++) {
//    writeEEPROM(address, str[str_idx], EEPROM_I2C_ADDRESS);
//    delay(5);
//    address += 1;
//  }
//  //pad 0 until 256 bytes
//  for (int str_idx = 0; str_idx < (256 - address); str_idx++) {
//    writeEEPROM(address, 0, EEPROM_I2C_ADDRESS);
//    //delay(5);
//    address += 1;
//  }
//  //-------end of writing EEPROM --------------
//
//  delay(1000);

  Serial.println("====");

  //-------read EEPROM --------------

  byte readVal1, readVal2;
  char *charptr;
  int cc_counter = 0;

  address = 0;

for (int idx = 0; idx < 256; idx++) {
      //read two (or a few) characters
      readVal1 = readEEPROM(address, EEPROM_I2C_ADDRESS);
      //byte tmp = readEEPROM(address, EEPROM_I2C_ADDRESS);
      address++;
      //Serial.print((char)readVal1);

      char buffer[40];
      //sprintf(buffer, "%08b", readVal1);

      sprintf(buffer,BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(readVal1));
      Serial.print(buffer);
      //Serial.print("%08b",readVal1,BIN);
      Serial.print("|");
      Serial.print((char)readVal1);
      Serial.print("|");
      Serial.println((int)readVal1);
};
return;



  for (int eeprom_entry_idx = 0; eeprom_entry_idx < 12; eeprom_entry_idx++) {
    cc_counter = 0;
    for (int idx = 0; idx < siemens_eeprom[eeprom_entry_idx].code_length; idx++) {
      //read two (or a few) characters
      readVal1 = readEEPROM(address, EEPROM_I2C_ADDRESS);
      //byte tmp = readEEPROM(address, EEPROM_I2C_ADDRESS);
      address++;
      //Serial.print((char)readVal1);
      Serial.print(readVal1,BIN);
      Serial.print("|");
      Serial.println((char)readVal1);
      return;
      if (readVal1 == 67) {
        cc_counter++;
      }; //two "C"?
      delay(5);
    };

    Serial.print("|");
    Serial.print(cc_counter);
    Serial.print("|");

    if (cc_counter == 2) {

      //read a two-byte number
      readVal1 = readEEPROM(address, EEPROM_I2C_ADDRESS);
      delay(5);
      address++;
      readVal2 = readEEPROM(address, EEPROM_I2C_ADDRESS);
      delay(5);
      address++;
      unsigned int data_length = (readVal1 << 8) + (readVal2 & 0xFF);
      Serial.print(data_length);

      Serial.print(":");
      //Serial.println(readVal1);
      //Serial.println(readVal2);

      //read a two-byte number (actual coil code)
      readVal1 = readEEPROM(address, EEPROM_I2C_ADDRESS);
      delay(5);
      address++;
      readVal2 = readEEPROM(address, EEPROM_I2C_ADDRESS);
      delay(5);
      address++;
      unsigned int coil_code = (readVal1 << 8) + (readVal2 & 0xFF);

      Serial.println(String(coil_code, HEX));
    }
    else {
      //read a two-byte number (the string length)
      readVal1 = readEEPROM(address, EEPROM_I2C_ADDRESS);
      delay(5);
      address++;
      readVal2 = readEEPROM(address, EEPROM_I2C_ADDRESS);
      delay(5);
      address++;
      int data_length = (readVal1 << 8) + (readVal2 & 0xFF);
      Serial.print(data_length);

      Serial.print(":");
      //read a string
      for (int idx = 0; idx < data_length; idx++) {
        readVal = readEEPROM(address, EEPROM_I2C_ADDRESS);
        delay(5);
        address++;
        Serial.print((char)readVal);
      }
      Serial.println("");
    }
  }
  //-------end of reading EEPROM --------------

  Serial.println("====");
}

void loop()
{

  // Nothing in loop

}
