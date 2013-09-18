/*
Copyright (c) 2013, Todd Stellanova, Rawthought Technologies LLC
Released under the terms of the GNU General Public License version 3 (GPLv3):

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/*
This software:

- Collects a list of active I2C devices on the bus at startup
- Periodically pings the collected list to determine whether any have stopped responding

It generates debug output on the serial port at 9600 baud: You can
connect to your arduino's serial port using eg "screen" on Mac OS X or Linux,
or any serial terminal emulator.

*/

#include <Wire.h>

//ALL CALL address for NXP PCA9663 &c.
#define LED_ALL_CALL_ADDRESS  0xE0


byte activeAddressList[127];
byte nDevices = 0;

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  Serial.println("\nSketch setup");
  
  memset(activeAddressList,0,sizeof(activeAddressList));
  nDevices = 0;
  scan();
  delay(5000);// wait 5 seconds before doiing anything else
}


void loop()
{
    byte rc, address;
     
    for (int i = 0; i < nDevices; ++i) {
      address = activeAddressList[i];
      Wire.beginTransmission(address);
      rc = Wire.endTransmission();
      if (0 != rc) {
          Serial.print("Ping error on address ");
          printHexAddress(address);
      }
      else {
          Serial.print("Device alive: ");
          printHexAddress(address);
      }
    }

 
     //delay between loop execution
     delay(5000);
}

void printHexAddress(byte address)
{
      Serial.print("0x");
      if (address < 16)  
        Serial.print("0");
      Serial.println(address,HEX); 
}

void scan()
{
    byte rc, address, maxAddress = 127;

    Serial.println("Scanning...");
    
    //first check to see whether devices are responding to the ALL CALL address
    address = LED_ALL_CALL_ADDRESS; 
    Wire.beginTransmission(address);
    rc = Wire.endTransmission();
    if (0 != rc) {
      Serial.println("LED_ALL_CALL_ADDRESS inactive");
    }
    else {
      Serial.println("LED_ALL_CALL_ADDRESS active");
    }
  
    //check all the I2C slave addresses to see which devices are present
    for (address = 1; address < maxAddress; ++address)   {
      //The act of begin/end an empty transmission effectively pings the address
      Wire.beginTransmission(address);
      rc = Wire.endTransmission();
      
      if (0 == rc)  {
        Serial.print("Found device at address: ");
        printHexAddress(address);
        activeAddressList[nDevices] = address;
        nDevices++;
      }
      else if (2 != rc) { //error code 2 indicates no device at address
        Serial.print("Error ");
        Serial.print(rc,DEC);
        Serial.print(" at address ");
        printHexAddress(address);
      }    
    }
  
    if (0 == nDevices) {
      Serial.println("No devices present!");
    }
    else {
     Serial.print(nDevices,DEC);
     Serial.println(" devices found!");
    }
  
}
