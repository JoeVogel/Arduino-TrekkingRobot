#include <Wire.h>
#include <HMC5883L.h>
#include <Compass.h>
#include <Motor.h>
#include <TinyGPS.h>

#define NORTH 0
#define TERMBAUD  9600
#define GPSBAUD  4800
#define PI 3.14159265358979323846

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
  Point p4;
} Points;


bool goTo(Point point);

//Declination Angle of Jaraguá do Sul is -0.31 and from Mauá is -0.35
const float declinationAngle = -0.31;

Points allToGo;
Point currentPoint;


void getgps(TinyGPS &gps, float *latitude, float *longitude);

bool inited = false;

void setup(){

  Serial.begin(TERMBAUD);
  Serial1.begin(GPSBAUD);
  
  pinMode(49,OUTPUT);
  digitalWrite(49,HIGH);

  compass.init(declinationAngle);

  motor.defineRight(3,2,4);
  motor.defineLeft(5,6,7);
  motor.defineCompass(compass,10);

  allToGo.p1.latitude = -26.46644;
  allToGo.p1.longitude = -49.11451;

  allToGo.p2.latitude = -26.46646;
  allToGo.p2.longitude = -49.11457;

  allToGo.p3.latitude = -26.46642;
  allToGo.p3.longitude = -49.11454;

}




void loop(){

  while(Serial1.available())     
  {
    int c = Serial1.read();  
    if(gps.encode(c))
    {

      getgps(gps, &currentPoint.latitude, &currentPoint.longitude);  

      if(!inited) {

        digitalWrite(49,LOW);
        delay(2000);
        inited = true;

      }

      /*Serial.print("Latitude: ");
      Serial.print(currentPoint.latitude, 5);
      Serial.print("  -   Longitude: ");
      Serial.print(currentPoint.longitude,5);   
      goToNorth();*/
      while(!goTo(allToGo.p1)); //BLUMENAU
      while(!goTo(allToGo.p2)); 
    }
  }

    /*while(!goTo(allToGo.p1)); //BLUMENAU
    while(!goTo(allToGo.p2)); //OTACILIO
    while(!goTo(allToGo.p3)); //PALHOCA*/


}



bool goTo(Point point) {

  float currentAngulation = compass.getCurrentAngulation();

  Serial.print("defining currentAngulation:");
  Serial.println(currentAngulation);
  delay(1000);

  Serial.print("defining turnToNorth:");
  Serial.println(currentAngulation);
  delay(1000);
  while(!motor.turnToNorth());


  /*Nunca modificar essa função*/
  float dlat = point.latitude - currentPoint.latitude;
  float dlong = point.longitude - currentPoint.longitude;
  float validationAngle = atan2(dlong,dlat) * 180 / PI;
  /*Até aqui*/

  int distlat = (point.latitude * 10000) - (currentPoint.latitude * 10000);
  int distlong = (point.longitude * 10000) - (currentPoint.longitude * 10000);

  if(distlat < 0) {
    distlat *= -1;
  }
  if(distlong < 0) {
    distlong *= -1; 
  }

  if(validationAngle < 0) {
    validationAngle *= -1;
    validationAngle = 360 - validationAngle;
  }

  Serial.print("distlat:");
  Serial.print(distlat);
  Serial.print("\tdistlong:");
  Serial.print(distlong);
  Serial.print("\tdefining validationAngle:");
  Serial.println(validationAngle);
  delay(1000);

  while(distlat > 5 && distlong > 5) {

    while(!motor.turnToDirection(validationAngle));

    distlat = (point.latitude * 10000) - (currentPoint.latitude * 10000);
    distlong = (point.longitude * 10000) - (currentPoint.longitude * 10000);

    currentAngulation = compass.getCurrentAngulation();
    motor.front(255);

    if(distlat < 0) {
      distlat *= -1;
    }
    if(distlong < 0) {
      distlong *= -1; 
    }
  }

  return true;
  delay(500);
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