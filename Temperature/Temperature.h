/*
  Temperature.h - Library used to gather temperature from OneWire sensor.
  Code by Bildr (http://bildr.org/2011/07/ds18b20-arduino/)
  Created on July 1, 2013.
*/

#ifndef Temperature_h
#define Temperature_h

#include "Arduino.h"
#include "OneWire.h"

class Temperature{
  public:
    Temperature();
    float getTemp(OneWire ds);
    
  private:
    uint8_t _pin;
    byte _data[12];
    byte _addr[8];
    byte _MSB;
    byte _LSB;
    byte _present;
    float _tempRead;
    float _TemperatureSum;
};

#endif