#include <LiquidCrystal_I2C.h>
#include <TinyGPS.h>
#include <Wire.h>

#define TERMBAUD  115200
#define GPSBAUD  4800

TinyGPS gps;
long longitudeDestino = -4911451, latitudeDestino = -2646639;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void getgps(TinyGPS &gps);

void setup()
{
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  
  // Sets baud rate of your terminal program
  Serial.begin(TERMBAUD);
  
  // Sets baud rate of your GPS
  Serial1.begin(GPSBAUD);
}

void loop()
{
  while(Serial1.available())     // While there is data on the RX pin...
  {
    int c = Serial1.read();    // load the data into a variable...

    if(gps.encode(c))      // if there is a new valid sentence...
    {
      getgps(gps);         // then grab the data.
      Serial.println("\n");
    }
  }
}

// The getgps function will get and print the values we want.
void getgps(TinyGPS &gps)
{
  float latitude, longitude;

  //Captura posição atual
  gps.f_get_position(&latitude, &longitude);

  //Escreve na tela de LCD
  lcd.setCursor(0,0);
  lcd.print(latitude,5);
  lcd.setCursor(0,1);
  lcd.print(longitude,5);
 
  //Bussola
  /*
  lcd.setCursor(0,0);
  lcd.print("Degrees:");
  lcd.setCursor(0,1);
  lcd.print(gps.f_course());
  delay(1000);*/

  /*if (longitudeDestino == (long)(longitude*100000) && latitudeDestino == (long)(latitude*100000)){
    for(int i = 0; i< 3; i++)
    {
      lcd.print("Achei");
    }
  }*/

}







