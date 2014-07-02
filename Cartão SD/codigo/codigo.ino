#include <SD.h>

Sd2Card card;
SdVolume volume;
SdFile root;
File myFile;

const int chipSelect = 53;    

void setup()
{
  Serial.begin(9600);
  Serial.print("\nInicializando Cartão SD...");
  pinMode(chipSelect, OUTPUT);     
   
  if (!SD.begin(chipSelect)) {
    Serial.println("Inicialização Falhou!!");
    return;
  }
  Serial.println("Inicializado.");
  
  // Abrindo o arquivo. Apenas um arquivo pode ser aberto por vez, se tiver algum arquivo aberto,
  // deve-se primeiro fechar o mesmo para que se possa abrir outro
    myFile = SD.open("test.txt", FILE_WRITE);
  
  // se o arquivo for aberto com sucesso, escreva nele.
  if (myFile) {
    Serial.print("Escrevendo no arquivo...");
    myFile.println("Olá mundo no arquivo txt.");
    // fechar o arquivo:
    myFile.close();
    Serial.println("Feito.");
  } else {
    // se o arquivo não abriu, imprima um erro:
    Serial.println("Erro ao abrir o arquivo.");
  }
  
  // reabrindo o arquivo para leitura:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    
    // Ler do arquivo se tiver algo nele:
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // fechar o arquivo:
    myFile.close();
  } else {
    // se o arquivo não abriu, imprima um erro:
    Serial.println("Erro ao abrir o arquivo.");
  }
}

void loop()
{
    
}
