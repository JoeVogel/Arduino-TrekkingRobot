#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

char movimentoDesignado;

RF24 radio(9,10);

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void)
{ 
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}

void loop(void)
{

  
  int y = analogRead(0);
  int x = analogRead(1);
  
  char entrada[20];
  sprintf(entrada,"y:%d;x:%d",y,x);
  
  radio.write(&entrada, sizeof(unsigned char[20]));
  
   
 }
