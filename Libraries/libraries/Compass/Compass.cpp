#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Compass.h>


void Compass::init(float declinationAngle) {
	this->declinationAngle = declinationAngle;
	this->mag = Adafruit_HMC5883_Unified(12345);
  	if(!this->mag.begin()) {
  		Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
  	}
  	sensor_t sensor;
  	this->mag.getSensor(&sensor);
  	delay(500);

}

/*
 * Utiliza um novo evento para o sensor
 * O valor do heading esta correto para quando as atribucoes sao reversas
 * Checa para encapsular devido a adicao da inclinacao
 * Converte de radianos para graus.
 * 
 * @return diferencaEmRelacaoAoNorte
 */
float Compass::getCurrentAngulation() {
 
  sensors_event_t event; 
  this->mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  heading += this->declinationAngle;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI; 
  return headingDegrees;

}