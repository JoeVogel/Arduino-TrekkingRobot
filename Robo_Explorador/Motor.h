#include "Arduino.h"

#ifndef Motor_h
#define Motor_h

class Motor {

public:

	int APower;
	int BPower;

	void defineRight(int a, int b, int c);
	void defineLeft(int a, int b, int c);

	void front(int power);
	void turn(int angle);
	void back(int power);
	void rightPower(int power);
	void leftPower(int power);
	void right(int power);
	void left(int power);
	void stop();


private:
	int motorARight;
	int motorALeft;
	int motorAPower;
	int motorBRight;
	int motorBLeft;
	int motorBPower;

	void _front();
	void _back();

};



#endif
