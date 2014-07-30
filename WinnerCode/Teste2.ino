#include <Wire.h>
#include "HMC5883L.h"
#include "Compass.h"
//#include "HMC6352.h"
#include "Motor.h"
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
  bool northDefined;
  bool checked;
  int sequence;
} Point;

float incrementRate = 0.00001;
int count = 5;
int acceleration = 80;
int chance[4] = {0,0,0,0};

int sensors[3] = {};

int use = 0;

//Declination Angle of Jaraguá do Sul is -0.31 and from Mauá is -0.35
const float declinationAngle = -0.35;

Point point[3];
Point backup[3];
Point currentPoint;


void getgps(TinyGPS &gps, float *latitude, float *longitude);

bool inited = false;

void setup(){

  Serial.begin(TERMBAUD);
  Serial1.begin(GPSBAUD);
  
  pinMode(49,OUTPUT);
  digitalWrite(49,HIGH);
  
  //pinMode(sensors[0],INPUT);
  //pinMode(sensors[1],INPUT);
  //pinMode(sensors[2],INPUT);
  
  defineFrontSonar();

  compass.init(declinationAngle);
  //compass.init();

  motor.defineRight(3,2,4);
  motor.defineLeft(5,6,7);
  motor.defineCompass(compass,20);

  point[0].latitude = -23.64675; //+2
  point[0].longitude = -46.57240;
  point[0].sequence = 1;
  point[0].checked = false;
  point[0].northDefined = false;
  backup[0] = point[0];
  
  point[1].latitude = -23.64692;
  point[1].longitude = -46.57252;
  point[1].sequence = 2;
  point[1].checked = false;
  point[1].northDefined = false;
  backup[1] = point[1];

  point[2].latitude = -23.64677;
  point[2].longitude = -46.57275;
  point[2].sequence = 2;
  point[2].checked = false;
  point[2].northDefined = false;
  backup[2] = point[2];

}




void loop(){
  
  //while(!goToObject());  
  //bool enter = false;
  //Serial.println(compass.getCurrentAngulation());
  //if(enter)
  while(Serial1.available())     
  {
    int c = Serial1.read();
    if(gps.encode(c))
    {

      getgps(gps, &currentPoint.latitude, &currentPoint.longitude);  
      //Serial.print("GPS LAT: ");Serial.print(currentPoint.latitude,5);
      //Serial.print("GPS LONG: ");Serial.println(currentPoint.longitude,5);
      
      if(!point[use].checked) {
        while(!motor.turnToNorth());
        //motor.front(0);
        point[use].checked = true;
        point[use].northDefined = true;
      } else {

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

        //Serial.print("DIST LAT: ");Serial.print(distlat);
        //Serial.print("DIST LONG: ");Serial.println(distlong);
  
  
        if(distlat < 6 || distlong < 6) {
          motor.threshold = 5;
        } else {
          motor.threshold = 10;
        }

        if(distlat >= 1 || distlong >= 1) {
          
          float angleReaded = compass.getCurrentAngulation();
          int diferrence = angleReaded - validationAngle;
          int rawDif = diferrence;

          //Serial.print("ANGLE: ");Serial.print(angleReaded);
          //Serial.print("DIRECTION: ");Serial.println(diferrence);
          
          if(diferrence < 0) {
            diferrence *= -1;
          }
            while (!motor.turnToDirection(validationAngle));
            motor.front(80);
            
            //findWhite();
            int distanceMiddle = getFrontDistance();
            if(distanceMiddle < 60 && distanceMiddle > 0) {
              if(distanceMiddle < 60 && distanceMiddle > 0){
                if(distanceMiddle < 60 && distanceMiddle > 0) {
                  stopAndSinalize();
                }
              }
            }
          //}
          
        } else {
          while(!goToObject());
          //Serial.println("STOP");
          motor.stop();
          //findWhite();
          /*digitalWrite(49,LOW);
          delay(1000);
          digitalWrite(49,HIGH);
          delay(1000);
          use++;*/ 
        }

      }

    }
  }

}




/*
 * Retorna a latitude e longitude em relacao ao ponto atual
 */
void getgps(TinyGPS &gps, float *latitude, float *longitude) {
  float latit = 99999, longi = 999999;
  gps.f_get_position(&latit, &longi);
  *latitude = latit;
  *longitude = longi;
}

//42,43,44,45 - pequenos
//9,8
int trigPin = 9;
int echoPin = 8;

void defineFrontSonar(){

  pinMode(trigPin,OUTPUT);  
  digitalWrite(trigPin,HIGH);//Poe em HIGH para que a primeira leitura seja correta                  
  pinMode(echoPin, INPUT);  

}

void defineRightSonar(){

pinMode(trigPin,OUTPUT); 
digitalWrite(trigPin,HIGH);//Poe em HIGH para que a primeira leitura seja correta                  
pinMode(echoPin, INPUT);

}

void defineLeftSonar(){

pinMode(trigPin,OUTPUT);  
digitalWrite(trigPin,HIGH);//Poe em HIGH para que a primeira leitura seja correta                   
pinMode(echoPin, INPUT);

}

int finbObject() {
  long distance = getFrontDistance();
  int count = 41;
  int finded = 0;
  while(finded == 0) {
    //Serial.println(distance);
    distance = getFrontDistance();
    //Serial.println(distance);
    if(distance < 170 && distance > 0) {
      distance = getFrontDistance();
      if(distance < 170 && distance > 0) {
        distance = getFrontDistance();
        if(distance < 170 && distance > 0) {
          finded = 1;
        }
      }
    }
    count--;
    digitalWrite(motor.motorBRight, LOW);
    digitalWrite(motor.motorBLeft, HIGH);
    digitalWrite(motor.motorARight, HIGH);
    digitalWrite(motor.motorALeft, LOW);
    motor.rightPower(200);
    motor.leftPower(200);
    delay(200);
    motor.rightPower(0);
    motor.leftPower(0);
    if(count == 1) {
      finded = -1;
      if(chance[0] == 0) {
        point[use] = backup[use];
        point[use].longitude += incrementRate;
        chance[0] = 1;
      } else if(chance[1] == 0) {
        point[use] = backup[use];
        point[use].latitude += incrementRate;
        chance[1] = 1;
      } else if(chance[2] == 0) {
        point[use] = backup[use];
        point[use].longitude -= incrementRate;
        chance[2] = 1;
      } else if(chance[3] == 0) {
        point[use] = backup[use];
        point[use].latitude -= incrementRate;
        chance[3] = 1;
      }
    }
    //findWhite();
  }
  return finded;
}

bool goToObject() {
  long distance = getFrontDistance();
  int response = 0;
  while(response == 0) {
    response = finbObject();
  };
  bool finded = false;
  while(!finded && response == 1) {
    Serial.println(distance);
    distance = getFrontDistance();
    if(distance < 40 && distance > 0) {
      finded = true;
    }
    if(distance > 150) {
      motor.front(80);
      delay(1500);
      motor.stop();
      finded = true;
    }
    digitalWrite(motor.motorBRight, HIGH);
    digitalWrite(motor.motorBLeft, LOW);
    digitalWrite(motor.motorARight, HIGH);
    digitalWrite(motor.motorALeft, LOW);
    motor.rightPower(80);
    motor.leftPower(80);
  }
  motor.rightPower(0);
  motor.leftPower(0);
  if(chance[3] == 1 && chance[2] == 1 && chance[1] == 1 && chance[0] == 1) {
      chance[3] = 0;
      chance[2] = 0;
      chance[1] = 0;
      chance[0] = 0;
      incrementRate = incrementRate + 0.00001;
  }
  if(finded) {
      stopAndSinalize();
  }
  
  //findWhite();
  
  if(response == -1)  finded = true;
  //Serial.println(response);
  return finded;
}

long getFrontDistance(){

  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  long DistanceMeasured= pulseIn(echoPin,LOW);	

  long result = 0;
  if(DistanceMeasured>150000){ 
    return 150000;   
  }
  else{
    result = (DistanceMeasured/50);
  }
  if(result > 3000) result = 3000;
  return result;
}

long getRightDistance(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin,HIGH);

  if (duration > 150000)
  {
    return 150000;
  }else{
    return duration /29 / 2 ;
  }
}

long getLeftDistance(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin,HIGH);

  if (duration > 150000)
  {
    return 150000;
  }else{
    return duration /29 / 2 ;
  }
}


void findWhite() {
  
  int s1 = digitalRead(sensors[0]);
  int s2 = digitalRead(sensors[1]);
  int s3 = digitalRead(sensors[2]);
  
  if(s1 == 1 || s2 == 1 || s3 == 1) {
    stopAndSinalize();
  } 
  
}


void stopAndSinalize() {
  chance[3] = 0;
  chance[2] = 0;
  chance[1] = 0;
  chance[0] = 0;
  motor.stop();
  incrementRate = 0.00001;
  digitalWrite(49,LOW);
  delay(1000);
  digitalWrite(49,HIGH);
  delay(1000);
  use++;
  motor.back(200,100);
  delay(2000);
  motor._front();
}
