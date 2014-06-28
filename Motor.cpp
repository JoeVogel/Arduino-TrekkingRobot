#include "Arduino.h"
#include "Motor.h"

void Motor::defineRight(double a, double b, double c) {
	this->motorARight = a;
	this->motorALeft = b;
	this->motorAPower = c;
	pinMode(a, OUTPUT);
	pinMode(b, OUTPUT);
	pinMode(c, OUTPUT);
}

void Motor::defineLeft(double a, double b, double c) {
	this->motorBRight = a;
	this->motorBLeft = b;
	this->motorBPower = c;
	pinMode(a, OUTPUT);
	pinMode(b, OUTPUT);
	pinMode(c, OUTPUT);
}

void Motor::front(double power) {
	this->APower = power;
	this->BPower = power;
	analogWrite(this->motorAPower, power);
	analogWrite(this->motorBPower, power);
	digitalWrite(this->motorARight, HIGH);
	digitalWrite(this->motorALeft, LOW);
	digitalWrite(this->motorBRight, HIGH);
	digitalWrite(this->motorBLeft, LOW);
}


void Motor::back(double power) {
	this->APower = power;
	this->BPower = power;
	analogWrite(this->motorAPower, power);
	analogWrite(this->motorBPower, power);
	digitalWrite(this->motorARight, LOW);
	digitalWrite(this->motorALeft, HIGH);
	digitalWrite(this->motorBRight, LOW);
	digitalWrite(this->motorBLeft, HIGH);
}

void Motor::whellRight(double power) {
	this->APower = power;
	analogWrite(this->motorAPower, power);
}

void Motor::whellLeft(double power) {
	this->BPower = power;
	analogWrite(this->motorBPower, power);
}


void Motor::turn(int angle){
	if(angle > 0) {
		if(this->motorAPower > 125) {
			this->APower = map(angle,0,90,125,255);
			analogWrite(this->motorAPower, this->APower);
			analogWrite(this->motorBPower, 125);	
		} else {
			this->APower = map(angle,0,90,125,0);
			Serial.println(this->APower);
			analogWrite(this->motorAPower, this->APower);
			analogWrite(this->motorBPower, 125);	
		}
	} else if(angle < 0) {
		if(this->motorAPower > 125) {
			this->BPower = map(angle,0,90,125,255);
			analogWrite(this->motorAPower, 125);
			analogWrite(this->motorBPower, this->BPower);	
		} else {
			this->APower = map(angle,0,90,0,125);
			analogWrite(this->motorAPower, 125);
			analogWrite(this->motorBPower, this->BPower);
		}
	}
}