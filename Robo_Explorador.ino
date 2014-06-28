#include "Motor.h"

Motor motor;
bool cont = true;

void setup() {

	Serial.begin(9600);
	motor.defineRight(8,7,9);
	motor.defineLeft(12,11,10);

}


void loop() {

	if(cont) {

		motor.front(50);
		delay(5000);
		motor.turn(90);
		delay(5000);
		for(int i = 0; i < 90; i++) {
			motor.turn(i);
			delay(100);
		}

		motor.front(150);
		delay(5000);
		motor.turn(90);
		delay(5000);
		for(int i = 0; i < 90; i++) {
			motor.turn(i);
			delay(100);
		}

		cont = false;
	}

}