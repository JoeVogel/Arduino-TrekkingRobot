#include "Arduino.h"
#include "Motor.h"
#include <Wire.h>
#include "HMC5883L.h"
#include "Compass.h"
//#include "HMC6352.h"

void Motor::defineCompass(Compass compass, int threshold) {
	this->compass = compass;
	this->threshold = threshold;
}

void Motor::defineRight(int a, int b, int c) {
	this->motorARight = a;
	this->motorALeft = b;
	this->motorAPower = c;
	pinMode(a, OUTPUT);
	pinMode(b, OUTPUT);
	pinMode(c, OUTPUT);
}

void Motor::defineLeft(int a, int b, int c) {
	this->motorBRight = a;
	this->motorBLeft = b;
	this->motorBPower = c;
	pinMode(a, OUTPUT);
	pinMode(b, OUTPUT);
	pinMode(c, OUTPUT);
}

void Motor::stop() {
	this->APower = 0;
	this->BPower = 0;
	analogWrite(this->motorAPower, 0);
	analogWrite(this->motorBPower, 0);
}

void Motor::front(int power) {
	this->APower = power;
	this->BPower = power;
	analogWrite(this->motorAPower, power);
	analogWrite(this->motorBPower, power);
	this->_front();
}

void Motor::_front() {
	digitalWrite(this->motorARight, HIGH);
	digitalWrite(this->motorALeft, LOW);
	digitalWrite(this->motorBRight, HIGH);
	digitalWrite(this->motorBLeft, LOW);
}


void Motor::back(int powerR, int powerL) {
	this->APower = powerR;
	this->BPower = powerL;
	analogWrite(this->motorAPower, powerR);
	analogWrite(this->motorBPower, powerL);
	this->_back();
}

void Motor::_back() {
	digitalWrite(this->motorARight, LOW);
	digitalWrite(this->motorALeft, HIGH);
	digitalWrite(this->motorBRight, LOW);
	digitalWrite(this->motorBLeft, HIGH);
}

void Motor::rightPower(int power) {
	this->APower = power;
	analogWrite(this->motorAPower, power);
}

void Motor::leftPower(int power) {
	this->BPower = power;
	analogWrite(this->motorBPower, power);
}

void Motor::right(int power) {
	Serial.println(power);
	if(power > 0) {
		digitalWrite(this->motorARight, HIGH);
		digitalWrite(this->motorALeft, LOW);
	} else {
		power = power * -1;
		digitalWrite(this->motorARight, LOW);
		digitalWrite(this->motorALeft, HIGH);
	}
	this->rightPower(power);
}

void Motor::left(int power) {
	if(power > 0) {
		digitalWrite(this->motorBRight, HIGH);
		digitalWrite(this->motorBLeft, LOW);
	} else {
		power = power * -1;
		digitalWrite(this->motorBRight, LOW);
		digitalWrite(this->motorBLeft, HIGH);
	}
	this->leftPower(power);
}

bool Motor::turnToNorth(){

	float angle = this->compass.getCurrentAngulation();
	_front();
	int acceleration = 200;
	if(angle > 10 && angle < 345) {
		while(angle > 10 && angle < 345) {
			angle = this->compass.getCurrentAngulation();
			//Serial.print("turnToNorth: ");
			//Serial.println(angle);
			if(angle > 180) {
				//Serial.println("__RIGHT__");
				digitalWrite(this->motorBRight, LOW);
				digitalWrite(this->motorBLeft, HIGH);
				digitalWrite(this->motorARight, HIGH);
				digitalWrite(this->motorALeft, LOW);
				rightPower(acceleration);
				leftPower(acceleration);
			} else {
				//Serial.println("__LEFT__");
				digitalWrite(this->motorBRight, HIGH);
				digitalWrite(this->motorBLeft, LOW);
				digitalWrite(this->motorARight, LOW);
				digitalWrite(this->motorALeft, HIGH);
				rightPower(acceleration);
				leftPower(acceleration);
			}
		}
	}else {
		rightPower(0);
		leftPower(0);
		return true;
	}

}

bool Motor::turnToDirection(float goalAngle) {
	float angle = this->compass.getCurrentAngulation();
	if(goalAngle > 365) {
		goalAngle = 0;
	}
	_front();
	int acceleration = 200;
	if(angle > (goalAngle + this->threshold) || angle < (goalAngle - this->threshold)) {

		while(angle > (goalAngle + this->threshold) || angle < (goalAngle - this->threshold)) {
			angle = this->compass.getCurrentAngulation();

			angle = this->compass.getCurrentAngulation();
			//Serial.print("turDirection: ");
			//Serial.print(angle);
			//Serial.print("goaDirection: ");
			//Serial.println(goalAngle);

			if(angle < goalAngle) {
				//Serial.println("__RIGHT__");
				digitalWrite(this->motorBRight, LOW);
				digitalWrite(this->motorBLeft, HIGH);
				digitalWrite(this->motorARight, HIGH);
				digitalWrite(this->motorALeft, LOW);
				rightPower(acceleration);
				leftPower(acceleration);
			} else {
				//Serial.println("__LEFT__");
				digitalWrite(this->motorBRight, HIGH);
				digitalWrite(this->motorBLeft, LOW);
				digitalWrite(this->motorARight, LOW);
				digitalWrite(this->motorALeft, HIGH);
				rightPower(acceleration);
				leftPower(acceleration);
			}
		}

	}else {
		return true;
	}
}


int Motor::defineQuadrant(int angulation) {

	int quadrant = -1;
    int quadrants[4][2];
    
    quadrants[0][0] = 0;
    quadrants[0][1] = 90;
    quadrants[1][0] = 91;
    quadrants[1][1] = 180;
    quadrants[2][0] = 181;
    quadrants[2][1] = 270;
    quadrants[3][0] = 271;
    quadrants[3][1] = 359;
    
    for(int i = 0; i < 4; i++) {
        if(angulation > quadrants[i][0] && quadrants[i][1] > angulation) {
            quadrant = i + 1;
            break;
        }
    }
    
    return quadrant;
    
}
