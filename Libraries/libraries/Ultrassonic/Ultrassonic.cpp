#include "Arduino.h"
#include "Ultrassonic_h"

void Sonar::defineFrontSonar(int URTRIG, int URPWM, char positionSensor){
	
	switch(positionSensor){

		case "f":
			this->URTRIG_f = URTRIG;
			this->URPWM_f = URPWM;

			pinMode(this->URTRIG_f,OUTPUT);                     
  			digitalWrite(this->URTRIG_f,HIGH);                  
  
  			pinMode(this->URPWM_f, INPUT);  
			break;
		case "r":
			this->URTRIG_r = URTRIG;
			this->URPWM_r = URPWM;

			pinMode(this->URTRIG_r,OUTPUT);                   
  			pinMode(this->URPWM_r, INPUT); 
			break;
		case "l":
			this->URTRIG_l = URTRIG;
			this->URPWM_l = URPWM;

			pinMode(this->URTRIG_l,OUTPUT);                   
  			pinMode(this->URPWM_l, INPUT);    
			break;
	}

}

long Sonar::getFrontDistance(){

	digitalWrite(this->URTRIG_f, LOW);
    digitalWrite(this->URTRIG_f, HIGH); 

	long DistanceMeasured= pulseIn(this->URPWM_f,LOW);	

	if(DistanceMeasured>200000){ 
      return 200000;   
   }
    else{
      return (DistanceMeasured/50);
   }
}

long Sonar::getLeftDistance(){

	digitalWrite(this->URTRIG_l, LOW);
  	delayMicroseconds(2);
  	digitalWrite(this->URTRIG_l, HIGH);
  	delayMicroseconds(10);
  	digitalWrite(this->URTRIG_l, LOW);
  	long duration = pulseIn(this->URPWM_l,HIGH);

  	if (duration > 200000)
  	{
  		return 200000
  	}else{
  		return duration /29 / 2 ;
  	}
}

long Sonar::getRightDistance(){

	digitalWrite(this->URTRIG_r, LOW);
  	delayMicroseconds(2);
  	digitalWrite(this->URTRIG_r, HIGH);
  	delayMicroseconds(10);
  	digitalWrite(this->URTRIG_r, LOW);
  	long duration = pulseIn(this->URPWM_r,HIGH);

  	if (duration > 200000)
  	{
  		return 200000;
  	}else{
  		return duration /29 / 2 ;
  	}
}



