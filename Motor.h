#include "Arduino.h"

#ifndef Motor_h
#define Motor_h

class Motor {

public:

	void defineRight(double a, double b, double c);
	void defineLeft(double a, double b, double c);

	void front(double power);
	void turn(int angle);
	void back(double power);
	void whellRight(double power);
	void whellLeft(double power);
	void stop();


private:
	int motorARight;
	int motorALeft;
	int motorAPower;
	double APower;
	int motorBRight;
	int motorBLeft;
	int motorBPower;
	double BPower;

};



#endif