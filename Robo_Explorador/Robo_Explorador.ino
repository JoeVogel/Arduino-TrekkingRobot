#include <Servo.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "Motor.h"

RF24 radio(9,10);
Motor motor;
const uint64_t pipe = 0xE8E8F0F0E1LL;
char movimentoDesignado[20];

void setup(){
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();

  motor.defineRight(4,3,2);
  motor.defineLeft(7,6,5);
}

int xref, yref;

void loop(){

  if (radio.available()) {
    bool done = false;
    while (!done)
    {
      done = radio.read( &movimentoDesignado, sizeof(unsigned char[20]));
      /********************************************************************/

      splitReturn(movimentoDesignado, &xref, &yref);
      Serial.println(yref);
      if(xref > 505) {
        int x = map(xref, 505, 1023, 0, 255);
        motor.front(x);
        makeY(yref);
      } 
      else if (xref < 495) {
        int x = map(xref, 0, 495, 0, 255);
        motor.back(255 - x);
        makeY(yref);
      } 
      else {
        if(yref > 550 || yref < 480) {
          if(yref > 550) {
            int y = map(yref, 505, 1023, 0, 255);
            motor.rightPower(0);
            motor.leftPower(y);
          } 
          else if (yref < 480) {
            int y = map(yref, 0, 495, 0, 255);
            motor.leftPower(0);
            motor.rightPower(y);
          }
        } 
        else {
          motor.stop();
        }
      } 
      /********************************************************************/
    }
  }
  else
  {
    Serial.println("Nenhum valor recebido");
  }
}


void makeY(int y) {

  if(yref > 550) {
    int y = map(yref, 505, 1023, 0, 255);
    motor.rightPower(255- y);
    motor.leftPower(y);
  } 
  else if (yref < 480) {
    int y = map(yref, 0, 495, 0, 255);
    motor.leftPower(y);
    motor.rightPower(255 - y);
  } 

}


void splitReturn(String receiveMessage,int *x, int *y) {

  int iGo = false;

  char index = ';';
  int commaPosition;
  int counter = 0;
  int arraySize;
  String temp = receiveMessage;
  String message = temp;
  while(temp.indexOf(index) != -1) {
    commaPosition = temp.indexOf(index);
    if(commaPosition != -1){
      temp = temp.substring(commaPosition+1, temp.length());
      arraySize++;
    }
    else{
      arraySize++;
    }
  };
  commaPosition = 0;
  String data[arraySize];
  do {
    commaPosition = message.indexOf(index);
    if(commaPosition != -1) {
      data[counter] = message.substring(0,commaPosition);
      message = message.substring(commaPosition+1, message.length());
      counter++;
    } 
    else {  // here after the last comma is found
      if(message.length() > 0)
        data[counter] = message;
    }
  }
  while(commaPosition >=0);

  for(int i = 0; i < 2; i++) {

    char index2 = ':';
    int commaPosition2;
    int counter2 = 0;
    int arraySize2;
    String temp2 = data[i];
    String message2 = temp2;
    while(temp2.indexOf(index2) != -1) {
      commaPosition2 = temp2.indexOf(index2);
      if(commaPosition2 != -1){
        temp2 = temp2.substring(commaPosition2 + 1, temp2.length());
        arraySize2++;
      }
      else{
        arraySize2++;
      }
    };
    commaPosition2 = 0;
    String data2[2];
    do {
      commaPosition2 = message2.indexOf(index2);
      if(commaPosition2 != -1) {
        data2[counter2] = message2.substring(0,commaPosition2);
        message2 = message2.substring(commaPosition2+1, message2.length());
        counter2++;
      } 
      else {  // here after the last comma is found
        if(message2.length() > 0)
          data2[counter2] = message2;
      }
    }
    while(commaPosition2 >=0);

    if(!iGo) {
      *y = data2[1].toInt();
      iGo = true;
    } 
    else {
      *x = data2[1].toInt();
    }
  }

}


