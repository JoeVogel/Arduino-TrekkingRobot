#include "Arduino.h"
#include <Motor.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <Compass.h>

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


void Motor::back(int power) {
	this->APower = power;
	this->BPower = power;
	analogWrite(this->motorAPower, power);
	analogWrite(this->motorBPower, power);
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
	if(angle > 10 && angle < 345) {
		while(angle > 10 && angle < 345) {
			angle = this->compass.getCurrentAngulation();
			Serial.print("turnToNorth: ");
			Serial.println(angle);
			if(angle < 220) {
				rightPower(0);
				leftPower(250);
			} else {
				rightPower(250);
				leftPower(0);
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
	Serial.println(angle);
	if(goalAngle > 365) {
		goalAngle = 0;
	}
	_front();
	if(angle > (goalAngle + this->threshold) || angle < (goalAngle - this->threshold)) {

		while(angle > (goalAngle + this->threshold) || angle < (goalAngle - this->threshold)) {
			angle = this->compass.getCurrentAngulation();
			Serial.print("turnToDirection - ");
			Serial.print("angle: ");
			Serial.print(angle);
			Serial.print("\tgoalAngle: ");
			Serial.println(goalAngle);


			if(angle < goalAngle || angle >= (360 - goalAngle)) {
				rightPower(100);
				leftPower(0);
			} else if(angle >= goalAngle || angle < (360 - goalAngle)) {
				rightPower(0);
				leftPower(100);
			}
		}

	}else {
		return true;
	}
}