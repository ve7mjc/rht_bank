/*	RHT03-Example-Serial.cpp
	Jim Lindblom <jim@sparkfun.com>
	August 31, 2015
    
    Ported to Arduino by Shawn Hymel
    October 28, 2016
	https://github.com/sparkfun/SparkFun_RHT03_Arduino_Library
	
	This a simple example sketch for the SparkFunRHT03 Ardiuno
	library.
	
	Looking at the front (grated side) of the RHT03, the pinout is as follows:
	 1     2        3       4
	VCC  DATA  No-Connect  GND
	
	Connect the data pin to Arduino pin D4. Power the RHT03 off the 3.3V bus.
	
	A 10k pullup resistor can be added to the data pin, though it seems to
	work without it.
	
    Development environment specifics:
	Arduino IDE v1.6.5
	Distributed as-is; no warranty is given.  
*/

#include <SparkFun_RHT03.h>

const int num_sensors = 8;
const unsigned int loop_time = 2000;
const int STATUS_LED = 13;

struct sensor {
  unsigned int data_pin;
  RHT03 rht;
} sensors[num_sensors];


void setup()
{

  sensors[0].data_pin = 2;
  sensors[1].data_pin = 6;
  sensors[2].data_pin = 24;
  sensors[3].data_pin = 25;
  sensors[4].data_pin = 26;
  sensors[5].data_pin = 27;
  sensors[6].data_pin = 28;
  sensors[7].data_pin = 29;

	Serial.begin(38400);

  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  for (int x = 0; x < num_sensors; x++) {    
      sensors[x].rht.begin(sensors[x].data_pin);
  } // forloop

  delay(2000); // delay minimum 2 seconds after power up
	
}

void loop()
{

	// Call rht.update() to get new humidity and temperature values from the sensor.
  unsigned long loopStartTime = millis();

  // pulse LED at beginning of sensor poll
  digitalWrite(STATUS_LED, HIGH);

  for (int x = 0; x < num_sensors; x++ ) {

      signed long updateReturn = sensors[x].rht.update();

      if (updateReturn == 1) {
      
        // The humidity(), tempC(), and tempF() functions can be called -- after 
        // a successful update() -- to get the last humidity and temperature
        Serial.println("RHT03," + String(x) + "," + String(sensors[x].rht.tempC()*10,0) + "," + String(sensors[x].rht.humidity()*10,0));
      
      } // if sensor read successful

  } // forloop, num_sensors

  digitalWrite(STATUS_LED, LOW);

  // ensure we do not complete this loop more than loop_time
  if ((millis() - loopStartTime) < loop_time)
    delay(loop_time-(millis() - loopStartTime));
  else
    delay(200); // delay for 200mS regardless

}
