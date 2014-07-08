#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Motor.h>
#include <TinyGPS.h>

#define NORTH 0
#define TERMBAUD  9600
#define GPSBAUD  4800

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
TinyGPS gps;


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




void setup(){

  Serial.begin(TERMBAUD);
  Serial1.begin(GPSBAUD);
  
  if(!mag.begin());

  allToGo.p1.latitude = 26.58912;
  allToGo.p1.longitude = 51.58954;
  allToGo.p2.latitude = 26.58632;
  allToGo.p2.longitude = 51.58912;
  allToGo.p3.latitude = 26.58986;
  allToGo.p3.longitude = 51.53423;
}




void loop(){
  
  if(Serial1.available())     
  {
    int c = Serial1.read();   
    if(gps.encode(c))
    {
      getgps(&currentPoint.latitude, &currentPoint.longitude);      
    }
  }

  Serial.print("Latitude: ");
  Serial.print(currentPoint.latitude);
  Serial.print("  -  Longitude: ");
  Serial.println(currentPoint.longitude);
  goToNorth();
  delay(500);
}


bool goToNorth() {
  float currentAngle = getCurrentAngulation();
  if(currentAngle > 180) {
    //Right
    Serial.println("Go Right");
  } else {
    //Left
    Serial.println("Go Left");
  }
}


/*
 * Utiliza um novo evento para o sensor
 * O valor do heading esta correto para quando as atribucoes sao reversas
 * Checa para encapsular devido a adicao da inclinacao
 * Converte de radianos para graus.
 * 
 * @return diferencaEmRelacaoAoNorte
 */
float getCurrentAngulation() {
 
  sensors_event_t event; 
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  heading += declinationAngle;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI; 
  return headingDegrees;

}


/*
 * Retorna a latitude e longitude em relacao ao ponto atual
 */
void getgps(float *latitude, float *longitude) {
  float latit, longi;
  gps.f_get_position(&latit, &longi);
  *latitude = latit;
  *longitude = longi;
}