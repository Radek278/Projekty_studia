const int analogInputPin = A0;
const int analogInputPin1 = A1;
const int analogInputPin2 = A2;
#define NIEBIESKA 3
#define ZIELONA 5
#define CZERWONA 6
#include <Wire.h>
#include <ACS712.h>
// Zmienna przechowująca odczytane napięcie
int sensorValue = 0;
int sensorValue2 = 0;
float napiecie=0.00;
float max= 5.00;
int rozdzielczosc=1023;
String wartosc="100%";
char buffer[10];
float voltagePerMilliamp = 0.001;
void oo()
{

  int x = analogRead(A1);
  delay(100);
  int x2 = analogRead(A2);
  delay(100);
  int x3 = analogRead(A3);
  delay(100);

  byte gora1 = (byte) (x & 0xFF);
  byte dol1 = (byte) ((x >> 8) & 0xFF);

  byte gora2 = (byte) (x2 & 0xFF);
  byte dol2 = (byte) ((x2 >> 8) & 0xFF);

  byte gora3 = (byte) (x3 & 0xFF);
  byte dol3 = (byte) ((x3 >> 8) & 0xFF);


  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(gora1);        // sends five bytes
  Wire.write(dol1);              // sends one byte
    Wire.write(gora2);        // sends five bytes
  Wire.write(dol2);              // sends one byte
    Wire.write(gora3);        // sends five bytes
  Wire.write(dol3);              // sends one byte
Wire.endTransmission(4);

  


  delay(200);
  

}
ACS712 SENSOR(ACS712_20A,A1);
// Funkcja setup, uruchamiana raz po uruchomieniu płytki Arduino
void setup() {
  // Inicjalizacja komunikacji szeregowej z prędkością 9600 bps
  Wire.begin();
  Serial.begin(9600);
}

// Funkcja loop, uruchamiana w pętli po funkcji setup
void loop() {
  // Odczytanie wartości napięcia na wejściu analogowym
  oo();


  
  sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  float skala=max/rozdzielczosc;
 
  // Wyświetlenie odczytanej wartości na monitorze szeregowym
  Serial.println("Odczytane napięcie: ");
  Serial.println(sensorValue*skala);
    Serial.println("Odczytane napięcie poprawnego: ");

  Serial.println(sensorValue*skala);//*(1200)/200);
  napiecie=(sensorValue*skala)*(1200/200);
  
  //Serial.print(napiecie);
  
  delay(1000);
  if (napiecie>12.75)
  {
    analogWrite(ZIELONA,0);
      analogWrite(CZERWONA,255);
      analogWrite(NIEBIESKA,255);
Serial.println("bateria :");
  wartosc="100%";
Serial.print(wartosc);
  }
  else if(napiecie<12.75 && napiecie>12.50)
  {
    analogWrite(ZIELONA,50);
      analogWrite(CZERWONA,255);
      analogWrite(NIEBIESKA,255);
    Serial.print("bateria :");
    wartosc="90%";
Serial.println(wartosc);
  }
   else if(napiecie<12.50 && napiecie>12.30)
  {
    analogWrite(ZIELONA,100);
      analogWrite(CZERWONA,255);
      analogWrite(NIEBIESKA,255);
    Serial.print("bateria :");
    wartosc="80%";
Serial.print(wartosc);
  }
   else if(napiecie<12.30 && napiecie>12.15)
  {
    analogWrite(ZIELONA,140);
    
     analogWrite(CZERWONA,255);
      analogWrite(NIEBIESKA,255);
    Serial.print("bateria :");
    wartosc="70%";
Serial.print(wartosc);
  }
   else if(napiecie<12.15 && napiecie>12.05)
  {
    analogWrite(ZIELONA,180);
    
     analogWrite(CZERWONA,255);
      analogWrite(NIEBIESKA,255);
    Serial.print("bateria :");
    wartosc="60%";
Serial.print(wartosc);
  }
   else if(napiecie<12.05 && napiecie>11.95)
  {
    analogWrite(ZIELONA,220);
     analogWrite(CZERWONA,255);
      analogWrite(NIEBIESKA,255);
    Serial.print("bateria :");
    wartosc="50%";
Serial.print(wartosc);
  }
   else if(napiecie<11.95 && napiecie>11.81 )
  {
    analogWrite(ZIELONA,255);
     analogWrite(CZERWONA,255);
      analogWrite(NIEBIESKA,0);
    Serial.print("bateria :");
    wartosc="40%";
Serial.print(wartosc);
  }
   else if(napiecie<11.81&& napiecie>11.66)
  {
      analogWrite(ZIELONA,255);
     analogWrite(CZERWONA,255);
      analogWrite(NIEBIESKA,230);
    Serial.print("bateria :");
    wartosc="30%";
Serial.print(wartosc);
  }
   else if(napiecie<11.66 && napiecie>11.51)
  {
        analogWrite(ZIELONA,255);
      analogWrite(NIEBIESKA,255);
    analogWrite(CZERWONA,0);
    Serial.print("bateria :");
    wartosc="20%";
Serial.print(wartosc);
  }
   else if(napiecie<11.51 && napiecie>10.50)
  {
           analogWrite(ZIELONA,255);
      analogWrite(NIEBIESKA,255);
    analogWrite(CZERWONA,220);
    Serial.print("bateria :");
    wartosc="10%";
Serial.print(wartosc);
  }
   else if(napiecie<10.50)
  {
           analogWrite(ZIELONA,255);
    
      analogWrite(NIEBIESKA,255);
   analogWrite(CZERWONA,0);
       delay(100);
       analogWrite(CZERWONA,255);
    Serial.print("bateria :");
    wartosc="0%";
Serial.print(wartosc);
  }
  else
  {
    Serial.print("brak akumulatora");
  }
  // Opóźnienie na 1 sekundę
  delay(1000);
}