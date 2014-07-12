#include "Arduino.h"
#include <Wire.h>
#include <HMC5883L.h>

#ifndef Compass_h
#define Compass_h

class Compass {

public:
	void init(float declinationAngle);
	float getCurrentAngulation();


private:
	HMC5883L mag;
	float declinationAngle;
	int error;
	int previous;
};

#endif