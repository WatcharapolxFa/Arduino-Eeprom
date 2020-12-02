/*
Use the I2C bus with EEPROM device
EEPROM 8 (Vcc) to Vcc
EEPROM 4 (GND) to GND
EEPROM 5 (SDA) to Arduino Analog Pin 4(SDA)
EEPROM 6 (SCL) to Arduino Analog Pin 5(SCL)
EEPROM 7 (WP) to GND
/* Memory length in bytes
24C01 = 128
24C02 = 256
24C04 = 512
24C08 = 1024
24C16 = 2048
24C64 = 8192
24C128= 16384
24C256= 32768
*/
#define memorylength 2048
#include <Wire.h>
byte deviceAddress; // Address of EEPROM chip
unsigned int eepromAddress;
void setup()
{
 byte eepromData;
 byte i;

 Serial.begin(9600); // Setup serial for debug
 Wire.begin(); // Start I2C bus
 for(i = 1; i < 127; i++ ) // sets the value (range from 1 to 127)
 {
 Wire.beginTransmission(i); // transmit to device
 if (Wire.endTransmission() == 0) // I2C devices found
 {
 deviceAddress = i;
 Serial.print("I2C Device Address: "); // Print Device Address
 Serial.println(deviceAddress, HEX); // print as an ASCII-encoded hexa);
 break;
 } 
 }
}
void loop()
{
  
}
