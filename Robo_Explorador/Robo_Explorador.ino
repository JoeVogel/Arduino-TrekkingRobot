#include "Motor.h"

Motor motor;

bool stop = false;

void setup() {

	Serial.begin(9600);
	motor.defineRight(8,7,9);
	motor.defineLeft(12,11,10);

}


void loop() {


	if(!stop) {

		motor.front(1);
		delay(4000);

		for(int i = 0; i < 120; i++) {
			//motor.right(i);
			delay(50);
		}

		motor.left(120);
		motor.right(120);
		delay(2000);
		motor.left(-120);
		motor.right(-120);
		delay(2000);

		motor.front(0);

		stop = true;

	}

}
