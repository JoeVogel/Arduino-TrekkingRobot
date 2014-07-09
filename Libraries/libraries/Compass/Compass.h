#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#ifndef Compass_h
#define Compass_h

class Compass {

public:
	void init(float declinationAngle);
	float getCurrentAngulation();


private:
	Adafruit_HMC5883_Unified mag;
	float declinationAngle;
};

#endif