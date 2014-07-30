#include <Wire.h> //I2C Arduino Library
#include <HMC6352.h>


HMC6352 compass;


void setup() {
  Serial.begin(9600);
  compass.init();
}

void loop() {
  Serial.print("Angle:");
  Serial.println(compass.getCurrentAngulation());
}


