/*
  Temperature.h - Library used to gather temperature from OneWire sensor.
  Code by Bildr (http://bildr.org/2011/07/ds18b20-arduino/)
  Created on July 1, 2013.
*/

#include "Arduino.h"
#include "Temperature.h"
#include "OneWire.h"

Temperature::Temperature(){
	
}

float Temperature::getTemp(OneWire ds){

	 if ( !ds.search(_addr)) {
	   //no more sensors on chain, reset search
	   ds.reset_search();
	   return -1000;
	 }

	 if ( OneWire::crc8( _addr, 7) != _addr[7]) {
	   Serial.println("CRC is not valid!");
	   return -1000;
	 }

	 if ( _addr[0] != 0x10 && _addr[0] != 0x28) {
	   Serial.print("Device is not recognized");
	   return -1000;
	 }

	 ds.reset();
	 ds.select(_addr);
	 ds.write(0x44,1); // start conversion, with parasite power on at the end

	 _present = ds.reset();
	 ds.select(_addr);  
	 ds.write(0xBE); // Read Scratchpad

 
	 for (int i = 0; i < 9; i++) { // we need 9 bytes
	  _data[i] = ds.read();
	 }
 
	 ds.reset_search();
 
	 _MSB = _data[1];
	 _LSB = _data[0];

	 _tempRead = ((_MSB << 8) | _LSB); //using two's compliment
	 _TemperatureSum = _tempRead / 16;
 
	 return _TemperatureSum;
}