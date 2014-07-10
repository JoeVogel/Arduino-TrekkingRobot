#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
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


void goTo(Point point);

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
  motor.defineCompass(compass,10);

  allToGo.p1.latitude = -26.91037;
  allToGo.p1.longitude = -49.08093;

  allToGo.p2.latitude = -27.48182;
  allToGo.p2.longitude = -50.12584;

  allToGo.p3.latitude = -27.64018;
  allToGo.p3.longitude = -48.68068;

  allToGo.p4.latitude = -27.11618;
  allToGo.p4.longitude = -49.64549;

  currentPoint.latitude = -27.21260;
  currentPoint.longitude = -49.64549;
}




void loop(){

  //motor.turnToNorth();
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

    //goTo(allToGo.p1); //BLUMENAU
    //goTo(allToGo.p2); //OTACILIO
    //goTo(allToGo.p3); //PALHOCA
    goTo(allToGo.p4); //TESTE

}



void goTo(Point point) {

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

    motor.turnToDirection(validationAngle);

    distlat = (point.latitude * 10000) - (currentPoint.latitude * 10000);
    distlong = (point.longitude * 10000) - (currentPoint.longitude * 10000);

    currentAngulation = compass.getCurrentAngulation();
    /*Serial.print("\tgo while angulation correct:");
    Serial.println(currentAngulation);*/

    if(distlat < 0) {
      distlat *= -1;
    }
    if(distlong < 0) {
      distlong *= -1; 
    }
  }

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