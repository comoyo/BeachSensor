/*
  This program takes input from two temp-sensors. 
  It displays current network info on two LEDs.
  Sensor data is transmitted to Xively using the GSM shield.
  LowPower library by rocketscream.com
  Author: Christian Dancke Tuen
*/

#include <GSM.h>                  // Importing all appropriate libraries
#include <HttpClient.h>
#include <Xively.h>
#include <OneWire.h> 
#include <Temperature.h>
#include <LowPower.h>

#define PINNUMBER "INSERT_PIN"          // SIM PIN
#define GPRS_APN "INSERT_APN"        // Access Point Name
#define GPRS_LOGIN "INSERT_USERNAME"     // Username (phone number)
#define GPRS_PASSWORD "INSERT_PASSWORD"  // Password (phone number)

GSMClient client;
GPRS gprs;
GSM gsmAccess;

int water_pin = 12;      // Water temperature sensor pin  (13)
int air_pin = 13;        // Air temperature sensor pin  (12)
int redLed = 10;         // Red LED pin
int greenLed = 8;       // Green LED pin
int wait = 0;

boolean notConnected = true;

OneWire water(water_pin);
OneWire air(air_pin);

#define FEED_ID INSERT_FEEDID
char xivelyKey[] = "INSERT_API_KEY";    // Set the Xively API key

char myWaterTempStream[] = "water";             // Set stream name (need to match Xively name)
char myAirTempStream[] = "air";                 // Set 2nd stream name


XivelyDatastream datastreams[] = {              // Create the datasterams
  XivelyDatastream(myWaterTempStream, strlen(myWaterTempStream), DATASTREAM_FLOAT),
  XivelyDatastream(myAirTempStream, strlen(myAirTempStream), DATASTREAM_FLOAT),
};

XivelyFeed feed(FEED_ID, datastreams, 2);       // Creating the feed, defining two datastreams
XivelyClient xivelyclient(client);              // Telling Xively to use the correct client

Temperature temp;
float temperature;

void setup(void) {                              // Connecting to network, initiating GPRS connection
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  digitalWrite(redLed, HIGH);                 // Red LED while not connected
  digitalWrite(greenLed, LOW);
}


void loop(void) {
  if(wait>=25){                                 // If slept 25 times
    startConnection();
    temperature = temp.getTemp(water);          // Get the temperature of the water
    datastreams[0].setFloat(temperature);       // Add temperature to Xively stream
    temperature = temp.getTemp(air);            // Get the temperature of the air
    datastreams[1].setFloat(temperature);
    int ret = xivelyclient.put(feed, xivelyKey);  // Send to Xively
    if(ret == 200){                             // If success, blink green LED
      digitalWrite(greenLed, LOW);
      delay(100);
      digitalWrite(greenLed, HIGH);
    }
    else{                                       // If something went wrong, blink red LED
      digitalWrite(redLed, HIGH);
      delay(100);
      digitalWrite(redLed, LOW);
    }
    closeConnection();
    wait=0;                                     // Reset sleep-counter
  }
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); // Enter power down state for 8 s with ADC and BOD module disabled
  wait++;
}

void startConnection(){
  while (notConnected) {
    digitalWrite(3,HIGH);                       // Enable the RX pin
    if(gsmAccess.begin(PINNUMBER)==GSM_READY){
      delay(3000);
      if(gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY){
        notConnected = false;
        digitalWrite(redLed, LOW);                // Green LED when connected
        digitalWrite(greenLed, HIGH);
      }
    }
    else{
      digitalWrite(redLed, HIGH);               // Red led to indicate some possible problems
      delay(1000);
    }
  }
}


void closeConnection(){
  while(notConnected==false){
    if(gsmAccess.shutdown()){
      delay(1000);
      digitalWrite(3,LOW);                    // Disable the RX pin
      notConnected = true;
      digitalWrite(redLed, LOW);              // Turn LEDs off
      digitalWrite(greenLed, LOW);
    }
    else{
      delay(1000);
    }
  }
}
