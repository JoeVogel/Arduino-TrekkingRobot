#include "Arduino.h"
#include "Motor.h"

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

void Motor::turn(int angle){
}
