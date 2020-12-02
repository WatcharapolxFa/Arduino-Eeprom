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
byte eepromData;
byte deviceAddress; // Address of EEPROM chip
unsigned int eepromAddress;

char arr[16] = {
     0x41,0x42, 0x43, 0x44, 0x45, 0x61, 0x62, 0x63, 0x64, 0x65, 0x31, 
     0x32, 0x33, 0x34, 0x35, 0x36 };
char arrr[4] = { 0x37, 0x38, 0x39, 0x30};

char str[] = " CE-KMITL";
     
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
         Serial.print("I2C Device Address : "); // Print Device Address
         Serial.println(deviceAddress, HEX); // print as an ASCII-encoded hexa);
         break;
       } 
     }
     
    eepromAddress = 0;
    eepromData = 128;
    writeEEPROM_byte(deviceAddress, eepromAddress, eepromData); // write to EEPROM
    Serial.println(readEEPROM_byte(deviceAddress, eepromAddress), HEX);
    
    writeEEPROM_page(deviceAddress, eepromAddress + 0 , (byte *)arr, sizeof(arr));
    writeEEPROM_page(deviceAddress, eepromAddress + 16, (byte *)arrr, sizeof(arrr));
    writeEEPROM_page(deviceAddress, eepromAddress + 20 , (byte *)str, sizeof(str));

    byte ch;
   Serial.println(" ");
   eepromAddress = 0; // first address
   ch = readEEPROM_byte(deviceAddress, eepromAddress); // access the first address from the memory
   while (ch!=0)
   {
   Serial.print((char) ch); // print content to serial port
   eepromAddress++; // increase address
   ch = readEEPROM_byte(deviceAddress, eepromAddress); // access an address from the memory
   }
   delay(2000); 

    Serial.println();
    dumpEEPROM(deviceAddress, 0, memorylength);
    delay(2000);
}

void writeEEPROM_byte(int device, unsigned int address, byte data )
{
   Wire.beginTransmission(device | (int)(address >> 8));
   Wire.write((int)(address & 0xFF)); // MSB
   Wire.write(data);
   Wire.endTransmission();
   delay(10);
}

byte readEEPROM_byte(int device, unsigned int address )
{
   byte rdata = 0;
   Wire.beginTransmission(device | (int)(address >> 8));
   Wire.write((int)(address & 0xFF)); // MSB
   Wire.endTransmission();
   Wire.requestFrom(device, 1);
   if (Wire.available())
   rdata = Wire.read();
   return rdata;
}

void writeEEPROM_page(int device, unsigned int address, byte* buffer, byte length )
{
 byte i;
 Wire.beginTransmission(device | (int)(address >> 8));
 Wire.write((int)(address & 0xFF)); // MSB
 for ( i = 0; i < length; i++)
 Wire.write(buffer[i]);
 Wire.endTransmission();
 delay(10);
} 

void dumpEEPROM(byte device, unsigned address, unsigned length)
{
   unsigned startAddress = address;
   unsigned stopAddress = address + length;
   for (unsigned i = startAddress; i < stopAddress; i += 16)
   {
   char buffer[16]; // page of EEPROM
   char temp[4];
   sprintf(temp, "%04x: ", i); // print address
   Serial.print(temp);
   readEEPROM_page(device, i, buffer, 16);
  
   for (int j = 0; j < 16; j++)
   {
   sprintf(temp, "%02x ", byte(buffer[j])); // print data (hexa)
   Serial.print(temp);
   }
   Serial.print(" ");
   for (int j = 0; j < 16; j++) // print data (ASCII)
   {
   if (isprint(buffer[j]))
   Serial.print(buffer[j]);
   else
   Serial.print('.');
   }
   Serial.println(" ");
   }
}

byte readEEPROM_page(int device, unsigned int address, byte *buffer, int length )
{
   byte i;
   Wire.beginTransmission(device | (int)(address >> 8));
   Wire.write((int)(address & 0xFF)); // MSB
   Wire.endTransmission();
   Wire.requestFrom(device, length);
   for ( i = 0; i < length; i++ )
   if (Wire.available())
   buffer[i] = Wire.read();
}

void loop()
{
   
}
