#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Compass.h>
#include <Motor.h>
#include <TinyGPS.h>

#define NORTH 0
#define TERMBAUD  9600
#define GPSBAUD  4800

TinyGPS gps;
Compass compass;
Motor motor;

typedef struct Point {
  float latitude;
  float longitude;
  float distance;
} Point;

typedef struct Points
{
  Point p1;
  Point p2;
  Point p3;
} Points;


//Declination Angle of Jaraguá do Sul is -0.31 and from Mauá is -0.35
const float declinationAngle = -0.31;

Points allToGo;
Point currentPoint;


void getgps(TinyGPS &gps, float *latitude, float *longitude);


void setup(){

  Serial.begin(TERMBAUD);
  Serial1.begin(GPSBAUD);
  
  compass.init(declinationAngle);

  motor.defineRight(4,3,2);
  motor.defineLeft(7,6,5);
  motor.defineCompass(compass);

  allToGo.p1.latitude = 26.58912;
  allToGo.p1.longitude = 51.58954;
  allToGo.p2.latitude = 26.58632;
  allToGo.p2.longitude = 51.58912;
  allToGo.p3.latitude = 26.58986;
  allToGo.p3.longitude = 51.53423;
}




void loop(){

  motor.turnToNorth();
  /*while(Serial1.available())     
  {
    int c = Serial1.read();  
    if(gps.encode(c))
    {
      getgps(gps, &currentPoint.latitude, &currentPoint.longitude);  
      Serial.print("Latitude: ");
      Serial.print(currentPoint.latitude, 5);
      Serial.print("  -   Longitude: ");
      Serial.print(currentPoint.longitude,5);   
      goToNorth(); 
    }
  }*/
}


bool goToNorth() {
  float currentAngle = compass.getCurrentAngulation();
  if(currentAngle > 180) {
    //Right
    Serial.print("    R: ");
    Serial.println(currentAngle);
  } else {
    //Left
    Serial.print("    L: ");
    Serial.println(currentAngle);
  }
}


/*
 * Retorna a latitude e longitude em relacao ao ponto atual
 */
void getgps(TinyGPS &gps, float *latitude, float *longitude) {
  float latit, longi;
  gps.f_get_position(&latit, &longi);
  *latitude = latit;
  *longitude = longi;
}