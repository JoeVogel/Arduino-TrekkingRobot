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
  bool checked;
  int sequence;
} Point;


int use = 1;

//Declination Angle of Jaraguá do Sul is -0.31 and from Mauá is -0.35
const float declinationAngle = -0.31;

Point point[3];
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

  point[0].latitude = -26.46648;
  point[0].longitude = -49.11451;
  point[0].sequence = 1;
  point[0].checked = false;

  point[1].latitude = -26.46651;
  point[1].longitude = -49.11464;
  point[1].sequence = 2;
  point[1].checked = false;

  point[2].latitude = -26.46646;
  point[2].longitude = -49.11457;
  point[2].sequence = 2;
  point[2].checked = false;

}




void loop(){

  while(Serial1.available())     
  {
    int c = Serial1.read();  
    if(gps.encode(c))
    {

      getgps(gps, &currentPoint.latitude, &currentPoint.longitude);  

      float currentAngulation = compass.getCurrentAngulation();

//      while(!motor.turnToNorth());

      /*Nunca modificar essa função*/
      float dlat = point[use].latitude - currentPoint.latitude;
      float dlong = point[use].longitude - currentPoint.longitude;
      float validationAngle = atan2(dlong,dlat) * 180 / PI;
      /*Até aqui*/

      int distlat = (point[use].latitude * 100000) - (currentPoint.latitude * 100000);
      int distlong = (point[use].longitude * 100000) - (currentPoint.longitude * 100000);

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

      Serial.print("LATITUDE:");Serial.print(currentPoint.latitude,5);
      Serial.print("LONGITUDE:");Serial.print(currentPoint.longitude,5);
      Serial.print("DIST_LAT:");Serial.print(distlat);
      Serial.print("\tDIST_LONG:");Serial.print(distlong);
      Serial.print("\tANGULO PARA IR:");Serial.println(validationAngle);
      delay(1000);

      if(distlat > 2 || distlong > 2) {

        while(!motor.turnToDirection(validationAngle));

        motor.front(70);
        delay(500);

        currentAngulation = compass.getCurrentAngulation();

        /*Nao modificar*/
        dlat = point[use].latitude - currentPoint.latitude;
        dlong = point[use].longitude - currentPoint.longitude;
        validationAngle = atan2(dlong,dlat) * 180 / PI;
        /*Nao modificar*/

        distlat = (point[use].latitude * 100000) - (currentPoint.latitude * 100000);
        distlong = (point[use].longitude * 100000) - (currentPoint.longitude * 100000);

        if(distlat < 0) {
          distlat *= -1;
        }
        if(distlong < 0) {
          distlong *= -1; 
        }
      }

    }
  }

    /*while(!goTo(allToGo.p1)); //BLUMENAU
    while(!goTo(allToGo.p2)); //OTACILIO
    while(!goTo(allToGo.p3)); //PALHOCA*/


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