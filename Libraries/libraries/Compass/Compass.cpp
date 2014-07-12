#include "Arduino.h"
#include <Wire.h>
#include <HMC5883L.h>
#include <Compass.h>


void Compass::init(float declinationAngle) {
	this->declinationAngle = declinationAngle;
  Wire.begin();
  this->previous = 0;
	this->mag = HMC5883L();
  this->error = this->mag.SetScale(1.3);
	if(this->error != 0) {
		Serial.println(this->mag.GetErrorText(this->error));
	}
  this->error = this->mag.SetMeasurementMode(Measurement_Continuous);
  if(this->error != 0) {
    Serial.println(this->mag.GetErrorText(this->error));
  }
  delay(500);

}

/*
 * Utiliza um novo evento para o sensor
 * O valor do heading esta correto para quando as atribucoes sao reversas
 * Checa para encapsular devido a adicao da inclinacao
 * Converte de radianos para graus.
 * 
 * @return diferencaEmRelacaoAoNorte
 */
float Compass::getCurrentAngulation() {

  int RoundDegreeInt;
  int PreviousDegree = this->previous;

 
  MagnetometerRaw raw = this->mag.ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = this->mag.ReadScaledAxis();

  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);

  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2� 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.009;
  heading += declinationAngle;

  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
  
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
  
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI;

  //correcting the angle issue
  if (headingDegrees >= 1 && headingDegrees < 240) 
  {
    headingDegrees = map(headingDegrees,0,239,0,179);
  }
  else if (headingDegrees >= 240)
  {
    headingDegrees =  map(headingDegrees,240,360,180,360);
  }

  //rounding the angle
  RoundDegreeInt =round(headingDegrees);

  //smoothing value
  if( RoundDegreeInt < (PreviousDegree + 3) && RoundDegreeInt > (PreviousDegree - 3) ) {
    RoundDegreeInt = PreviousDegree;
  }

  PreviousDegree = RoundDegreeInt;

  return RoundDegreeInt;

}