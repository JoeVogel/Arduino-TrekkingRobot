int trigPin = 7;
int echoPin = 9;

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

long getFrontDistance(){

  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  long DistanceMeasured= pulseIn(echoPin,LOW);	

  if(DistanceMeasured>150000){ 
    return 150000;   
  }
  else{
    return (DistanceMeasured/50);
  }
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

void setup(){

  Serial.begin(9600);
  defineLeftSonar();
}

void loop(){

  Serial.print("Distancia em CM: ");
  Serial.println(getLeftDistance());
  delay(150);

}


