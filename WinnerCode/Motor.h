#include "Arduino.h"
#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_HMC5883_U.h>
#include "Compass.h"
//#include "HMC6352.h"

#ifndef Motor_h
#define Motor_h

class Motor {

public:

	int APower;
	int BPower;

	void defineRight(int a, int b, int c);
	void defineLeft(int a, int b, int c);
	void defineCompass(Compass compass, int threshold);

	void front(int power);
	bool turnToNorth();
	bool turnToDirection(float goalAngle);
	void back(int powerR, int powerL);
	void rightPower(int power);
	void leftPower(int power);
	void right(int power);
	void left(int power);
	void stop();
	void _front();
	void _back();
	int defineQuadrant(int angulation);
	int threshold;
	int motorARight;
	int motorALeft;
	int motorAPower;
	int motorBRight;
	int motorBLeft;
	int motorBPower;
	Compass compass;

};



#endif
