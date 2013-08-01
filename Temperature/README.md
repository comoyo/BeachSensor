# Temperature library for Arduino

This library makes it easy to get the temperature from one-wire temperature sensors. I created this library to minimize the amount of code needed to get a basic temperature reading from the sensor.

## Basic usage:

```Arduino
	#include <Temperature.h>
	#include <OneWire.h> 

	int SENSOR_PIN = 3;
	OneWire ds(SENSOR_PIN);
	
	Temperature temp;
	float temperature = temp.getTemp(ds);
```