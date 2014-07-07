#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Motor.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

<<<<<<< HEAD
//Declination Angle of Jaraguá do Sul is -0.31 and from Mauá is -0.35
const float declinationAngle = -0.31;

=======
>>>>>>> 50a77c91bc8baa1cbcbd72ab647a7aacd9187e13
void setup(){
  Serial.begin(9600);
  
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
  }
}

void loop(){
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mag.getEvent(&event);
  
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  if(headingDegrees > 1 && headingDegrees <= 180)Serial.println("vire a esquerda"); 
  else if(headingDegrees> 180 && headingDegrees <= 359)Serial.println("vire a direita");
  else if(headingDegrees < 1)Serial.println("NORTE!!!");
  
  delay(500);
}
