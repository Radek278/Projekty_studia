#include <TimerOne.h>
#include <Timers.h>
#include <Wire.h>
#include "ACS712.h"
#define VODKA_PIN 2// 295-300ml wódka
#define RUM_PIN 3//295-300ml rum
#define TEQUILA_PIN 4//295-300 ml tequilla 
#define LIKIER_PIN 5 //295-300 ml ochwe wiecej jak 1 raz sie leje 
//#define TEN_PIN 7
#define  AKUMULATORKURWATEN 6
#define Zasilacz 7
#define Lodowka 8
#define Trig  12  //pin 2 Arduino połączony z pinem Trigger czujnika
#define Echo  11  //pin 3 Arduino połączony z pinem Echo czujnika
Timers <8> akcja;


ACS712 sensor(ACS712_20A, A1);
ACS712 sensor2(ACS712_20A, A2);
ACS712 sensor3(ACS712_20A, A3);
ACS712 sensor4(ACS712_20A, A4);
const int analogInputPin = A0;//napiecie aku




int CM;        //odległość w cm
long CZAS;     //długość powrotnego impulsu w uS
int state=0;
int time=0;
int time_end1;
int time_end2;
int time_end3;
int time_end4;
int time_test;
float nap;
float nap_zas;
float Uaku=8.0;
float Iwyj=24;
float Up1=33;
float Up2=42;
float Up3=36;
float Up4=20;
float Ip1=34;
float Ip2=24;
float Ip3=32;
float Ip4=42;
int sensorValue = 0;
float napiecie=0.0;
int rozdzielczosc=1021;
float max=5.0;
int tmp=77;

void receiveEvent(int howMany)
{
  while (Wire.available()) {
    float skala2=max/rozdzielczosc;
    byte dolnyBajt1 = Wire.read();
    byte gornyBajt1 = Wire.read();
    int x1 = (gornyBajt1 << 8) | dolnyBajt1; // receive byte as an integer
    //Serial.print("X1: "); // Print label for X1
    //Serial.println(x1);    // print the integer
    Up1 = x1;
    Up1 = Up1 *skala2*6;
        byte dolnyBajt2 = Wire.read();
    byte gornyBajt2 = Wire.read();
    int x2 = (gornyBajt2 << 8) | dolnyBajt2; // receive byte as an integer
    //Serial.print("X2: "); // Print label for X1
    //Serial.println(x2);    // print the integer
        Up2 = x2;
        Up2 = Up2 *skala2*6;


        byte dolnyBajt3 = Wire.read();
    byte gornyBajt3 = Wire.read();
    int x3 = (gornyBajt3 << 8) | dolnyBajt3; // receive byte as an integer
    //Serial.print("X3: "); // Print label for X1
    //Serial.println(x3);    // print the integer
        Up3 = x3;
        Up3 = Up3 *skala2*6;

  }
}


void setup() {
 sensor.calibrate();
  sensor2.calibrate();
  sensor3.calibrate();
  sensor4.calibrate();


    pinMode(Trig, OUTPUT);                     //ustawienie pinu 12 w Arduino jako wyjście
    pinMode(Echo, INPUT);                      //ustawienie pinu 11 w Arduino jako wejście

  pinMode(VODKA_PIN,OUTPUT);
  pinMode(RUM_PIN,OUTPUT);
    pinMode(TEQUILA_PIN,OUTPUT);
      pinMode(LIKIER_PIN,OUTPUT);
      pinMode(Lodowka,OUTPUT);

pinMode(AKUMULATORKURWATEN ,OUTPUT);
  pinMode(Zasilacz,OUTPUT);
  digitalWrite(RUM_PIN,HIGH);
  digitalWrite(VODKA_PIN,HIGH);
  digitalWrite(TEQUILA_PIN,HIGH);
  digitalWrite(LIKIER_PIN,HIGH);
  digitalWrite(AKUMULATORKURWATEN,HIGH);
  digitalWrite(Zasilacz,HIGH);
  digitalWrite(Lodowka,HIGH);

Wire.begin(4); 
     Wire.onReceive(receiveEvent);
  // put your setup code here, to run once:
   
  Serial.begin(9600); 
   // przypisanie funkcji callback1 do pierwszego przerwania

 
// Wątek 1: wcześniej zdefiniowana funkcja o nazwie zadanie1, wywoływana co ok  1 sek.
  akcja.attach(1,1050,pomiar);
  akcja.attach(2,33,pompki);
  //kalibracja Timera 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10); //prescaler 8 ~32ms (110p)
  TIMSK1 |= (1 << TOIE1);


}

void loop() {
  // put your main code here, to run repeatedly:
 // Checks whether data is comming from the serial port
 //float nap=nap_aku();
// if (nap<8.00)
// {

//   digitalWrite(AKUMULATORKURWATEN ,HIGH); 
//   delay (400);
//     digitalWrite(Zasilacz ,LOW); 
// }
// else 
// {
//   digitalWrite(Zasilacz ,HIGH); 
//  delay (400);
//   digitalWrite(AKUMULATORKURWATEN ,LOW); 
  
 
       
//}




 akcja.process();
 //pomiar();
    
if(Serial.available() > 0){
  
     // Checks whether data is comming from the serial port
    state = Serial.read(); // Reads the data from the serial port

    
}


}






void pomiar()
{
//   Ip1 = sensor.getCurrentDC() - 0.38;
//  delay(100);
//   Ip2 = sensor2.getCurrentDC() - 0.38;
//  delay(100);
//   Ip3 = sensor3.getCurrentDC() - 0.38;
//    delay(100);
//   Ip4 =sensor4.getCurrentDC() - 0.38;
  
  if (Ip1 < 0.09) {
    Ip1 = 0;
  }
 
  if (Ip2 < 0.09) {
    Ip2= 0;
  }
   if (Ip3 < 0.09) {
    Ip3 = 0;
  }
 if (Ip4 < 0.09) {
    Ip4 = 0;
  }
// nap=nap_aku();
// if (nap<8.00)
// {

//   digitalWrite(AKUMULATORKURWATEN ,HIGH); 

//     digitalWrite(Zasilacz ,LOW); 
// }
// else 
// {
//   // digitalWrite(Zasilacz ,HIGH); 

//   // digitalWrite(AKUMULATORKURWATEN ,LOW); 
//   // state=103;
//   // state=104;
  
 
       
// }

// nap_zas=Up2;
// if (nap_zas>9.80 && nap_zas < 13.00)
// {

//   digitalWrite(AKUMULATORKURWATEN ,HIGH); 

//   digitalWrite(Zasilacz ,LOW); 
// }
// else
// {
//     digitalWrite(AKUMULATORKURWATEN ,LOW); 

//   digitalWrite(Zasilacz ,HIGH); 
// }


nap=nap_aku();
  int procent=akunal(nap);
  // tu ma byc pin od akumulatora a nie vodka_pin
  // if(Up1 < 8){
  //   digitalWrite(VODKA_PIN,HIGH);
  // }
  // else
  // {
  //   digitalWrite(VODKA_PIN,LOW);
  // }
Serial.print(nap);
  Serial.print(" V");
  Serial.print("|");
 Serial.print(Iwyj);
  Serial.print(" A");
  Serial.print("|");
   Serial.print(Up1);
  Serial.print(" V");
    Serial.print("|");
      Serial.print(Up2);
  Serial.print(" V");
    Serial.print("|");
     Serial.print(Up3);
  Serial.print(" V");
    Serial.print("|");
     Serial.print(Up4);
  Serial.print(" V");
    Serial.print("|");
  //    Serial.print(Ip1);
  // Serial.print(" A");
  //   Serial.print("|");
  //   Serial.print(Ip2);
  // Serial.print(" A");
  //   Serial.print("|");
  //   Serial.print(Ip3);
  // Serial.print(" A");
  //   Serial.print("|");
  //   Serial.print(Ip4);
  // Serial.print(" A");
  // Serial.print("|");
    Serial.print(procent);
  Serial.print(" %");
  Serial.print("|");
 
//if(state==103 or state==104 or state==105 or state==106 or state==107 or state== 108)
//{//

     Serial.print(Ip1);
  Serial.print(" A");
    Serial.print("|");
    Serial.print(Ip2);
  Serial.print(" A");
    Serial.print("|");
    Serial.print(Ip3);
  Serial.print(" A");
    Serial.print("|");
    Serial.print(Ip4);
  Serial.print(" A");
    Serial.print("|");

//}
    
 
 

 




}


float nap_aku()
{
  sensorValue = analogRead(analogInputPin);//a0
  float skala=max/rozdzielczosc;
  napiecie=(sensorValue*skala)*(1200 / 200);
  return napiecie;
}


float akunal(float napiecie) // okresla poziom naladowania akumulatora
{
  
if (napiecie>12.75)
{
  return 100;
}
else if(napiecie<12.75 && napiecie>12.50)
{
  return 90;
}
else if(napiecie<12.50 && napiecie>12.30)
{
  return 80;
}

 else if(napiecie<12.30 && napiecie>12.15)
 {
  return 70;
 }
 else if(napiecie<12.15 && napiecie>12.05)
 {
  return 60;
 }

 else if(napiecie<12.05 && napiecie>11.95)
 {
  return 50;
 }
 else if(napiecie<11.95 && napiecie>11.81 )
 {
  return 40;
 }
 else if(napiecie<11.81&& napiecie>11.66)
 {
 // digitalWrite(Lodowka,HIGH);
  return 30;
 }
  else if(napiecie<11.66 && napiecie>11.51)
  {
  return 20;
  //digitalWrite(Lodowka,HIGH);
  }
  else if(napiecie<11.51 && napiecie>10.50)
  {
  return 10;
  digitalWrite(Lodowka,HIGH);
  }
  else if(napiecie<10.50)
  {
    return 0;
  digitalWrite(Lodowka,HIGH);
  }
}

 void pomiar_odleglosci ()
 {
   digitalWrite(Trig, LOW);        //ustawienie stanu wysokiego na 2 uS - impuls inicjalizujacy - patrz dokumentacja
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);       //ustawienie stanu wysokiego na 10 uS - impuls inicjalizujacy - patrz dokumentacja
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  digitalWrite(Echo, HIGH); 
  CZAS = pulseIn(Echo, HIGH);
  CM = CZAS / 58;                //szerokość odbitego impulsu w uS podzielone przez 58 to odleglosc w cm - patrz dokumentacja
}

//przerwania timera 1
//ISR(TIMER1_OVF_vect){
  void pompki(){
  //Serial.println(millis());
pomiar_odleglosci();

if(CM < 15){
  

  switch (state)
  {
  case 0:
  digitalWrite(VODKA_PIN,HIGH);
  digitalWrite(RUM_PIN,HIGH);
  digitalWrite(LIKIER_PIN,HIGH);
  digitalWrite(TEQUILA_PIN,HIGH);

  break;

  case 103: // vodka + cola 1/3 do 2/3
  time = 0;
  time_end1 = 35; //

  state = 11;


 digitalWrite(VODKA_PIN,LOW);//30   ml
  digitalWrite(RUM_PIN,LOW);//30 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
  digitalWrite(TEQUILA_PIN,LOW);//30 ml

  break;
  case 104: // vodka + cola 1/3 do 2/3

  digitalWrite(LIKIER_PIN,LOW);//30 ml
  digitalWrite(TEQUILA_PIN,LOW);//60 ml
  time = 0;
  time_end1 = 35; //30
  time_end2 = 65; // 60
  state = 12;

  //time_test = 200;
 // state = 222;

  digitalWrite(LIKIER_PIN,LOW);
  digitalWrite(TEQUILA_PIN,LOW);
  //Serial.println("test 1");

//ignoring the value below 0.09
  break;

 case 105: // vodka + cola 1/3 do 2/3
  time = 0;
  time_end1 = 45; //

  state = 13;

  //time_test = 200;
 // state = 222;

 digitalWrite(VODKA_PIN,LOW);//40   ml
  digitalWrite(RUM_PIN,LOW);//40 ml
  digitalWrite(LIKIER_PIN,LOW);//40 ml
  //digitalWrite(TEQUILA_PIN,LOW);//30 ml

  //Serial.println("test 1");


  break;


  case 106: // vodka + cola 1/3 do 2/3//MAI TAI
  time = 0;
  time_end1 = 35; //
  time_end2=55;
  state = 14;

 digitalWrite(VODKA_PIN,LOW);//30   ml
  digitalWrite(RUM_PIN,LOW);//50 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
  //digitalWrite(TEQUILA_PIN,LOW);//30 ml

  break;


case 107: // vodka + cola 1/3 do 2/3//MAI TAI//5
  time = 0;
  time_end1 = 35; //
  time_end2=65;
  state = 15;

 //digitalWrite(VODKA_PIN,LOW);//30   ml
  digitalWrite(RUM_PIN,LOW);//60 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
  digitalWrite(TEQUILA_PIN,LOW);//30 ml

  break;



case 108: // vodka + cola 1/3 do 2/3//MAI TAI//6
  time = 0;
  time_end1 = 35; //
  time_end2=55;
  state = 16;

 digitalWrite(VODKA_PIN,LOW);//50  ml
  digitalWrite(RUM_PIN,LOW);//30 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
 // digitalWrite(TEQUILA_PIN,LOW);//30 ml

  break;




case 109: // vodka + cola 1/3 do 2/3//MAI TAI//7
  time = 0;
  time_end1 = 45; //
  time_end2=65;
  state = 17;

 digitalWrite(VODKA_PIN,LOW);//50  ml
  digitalWrite(RUM_PIN,LOW);//30 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
 // digitalWrite(TEQUILA_PIN,LOW);//30 ml

  break;


case 110: // vodka + cola 1/3 do 2/3//MAI TAI//8
  time = 0;
  time_end1 = 35; //
  time_end2=65;
  state = 18;

 //digitalWrite(VODKA_PIN,LOW);//50  ml
  digitalWrite(RUM_PIN,LOW);//60 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
 digitalWrite(TEQUILA_PIN,LOW);//30 ml

  break;

case 111: // vodka + cola 1/3 do 2/3//MAI TAI//9
  time = 0;
  time_end1 = 35; //
  time_end2=55;
  state = 19;

 //digitalWrite(VODKA_PIN,LOW);//50  ml
  digitalWrite(RUM_PIN,LOW);//30 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
 digitalWrite(TEQUILA_PIN,LOW);//60 ml

  break;


case 112: // vodka + cola 1/3 do 2/3//MAI TAI//10
  time = 0;
  time_end1 = 35; //
  time_end2=65;
  state = 20;

 //digitalWrite(VODKA_PIN,LOW);//50  ml
  digitalWrite(RUM_PIN,LOW);//30 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
 digitalWrite(TEQUILA_PIN,LOW);//60 ml

  break;





  case 113: // vodka + cola 1/3 do 2/3//MAI TAI//11
  time = 0;
  time_end1 = 45; //
  time_end2=65;
  state = 21;

 //digitalWrite(VODKA_PIN,LOW);//50  ml
  digitalWrite(RUM_PIN,LOW);//60 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
 digitalWrite(TEQUILA_PIN,LOW);//30 ml

  break;



case 114: // vodka + cola 1/3 do 2/3//MAI TAI//12
  time = 0;
  time_end1 = 35; //
  time_end2=65;
  state = 22;

 digitalWrite(VODKA_PIN,LOW);//60 ml
  digitalWrite(RUM_PIN,LOW);//30 ml
  digitalWrite(LIKIER_PIN,LOW);//30 ml
 //digitalWrite(TEQUILA_PIN,LOW);//30 ml

  break;




  case 11:
  time++;
  if(time >= time_end1){
     digitalWrite(VODKA_PIN,HIGH);
  digitalWrite(RUM_PIN,HIGH);
  digitalWrite(LIKIER_PIN,HIGH);
  digitalWrite(TEQUILA_PIN,HIGH);
    
   // Serial.println("Test 2");
    time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
    // if(time >= time_end2){
    //   digitalWrite(RUM_PIN,HIGH);
    //   time = 0;
    //   time_end1=0;
    //   time_end2=0;
    //   state = 0;
    //   //Serial.println("Test 3");
    // }
  }
  break;





case 12:
  time++;
  if(time >= time_end1){
    digitalWrite(LIKIER_PIN,HIGH);
   // Serial.println("Test 2");
    if(time >= time_end2){
      digitalWrite(TEQUILA_PIN,HIGH);
      time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
      //Serial.println("Test 3");
    }
  }
  break;

  case 13:
  time++;
  if(time >= time_end1){
   
 digitalWrite(VODKA_PIN,HIGH);//40   ml
  digitalWrite(RUM_PIN,HIGH);//40 ml
  digitalWrite(LIKIER_PIN,HIGH);//40 ml
      time = 0;
      time_end1=0;
      //time_end2=0;
      state = 0;
      //Serial.println("Test 3");
   // }
  }
  break;



case 14:
  time++;
  if(time >= time_end1){
   
 digitalWrite(VODKA_PIN,HIGH);//30   ml
  digitalWrite(LIKIER_PIN,HIGH);//30 ml
  
      
      //Serial.println("Test 3");
   // }
   if(time >= time_end2)
   {

 digitalWrite(RUM_PIN,HIGH);//50 ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;



case 15:
  time++;
  if(time >= time_end1){
   
//  digitalWrite(VODKA_PIN,HIGH);//30   ml
//   digitalWrite(LIKIER_PIN,HIGH);//30 ml
  
  digitalWrite(LIKIER_PIN,HIGH);//30 ml
  digitalWrite(TEQUILA_PIN,HIGH);//30 ml
      
      //Serial.println("Test 3");
   // }
   if(time >= time_end2)
   {

 digitalWrite(RUM_PIN,HIGH);//60 ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;





case 16:
  time++;
  if(time >= time_end1){
  digitalWrite(RUM_PIN,HIGH);//30 ml
  digitalWrite(LIKIER_PIN,HIGH);//30 ml
   if(time >= time_end2)
   {

 digitalWrite(VODKA_PIN,HIGH);//50  ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;




case 17:
  time++;
  if(time >= time_end1){

 digitalWrite(VODKA_PIN,HIGH);//30  ml
  
  digitalWrite(LIKIER_PIN,HIGH);//30 ml
   if(time >= time_end2)
   {

 digitalWrite(RUM_PIN,HIGH);//60 ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;




case 18:
  time++;
  if(time >= time_end1){


  digitalWrite(LIKIER_PIN,HIGH);//30 ml
 digitalWrite(TEQUILA_PIN,HIGH);//30 ml
   if(time >= time_end2)
   {

digitalWrite(RUM_PIN,HIGH);//60 ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;



case 19:
  time++;
  if(time >= time_end1){


    digitalWrite(RUM_PIN,HIGH);//30 ml
  digitalWrite(LIKIER_PIN,HIGH);//30 ml
 
   if(time >= time_end2)
   {

digitalWrite(TEQUILA_PIN,HIGH);//60 ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;





case 20:
  time++;
  if(time >= time_end1){


      digitalWrite(RUM_PIN,HIGH);//30 ml
  digitalWrite(LIKIER_PIN,HIGH);//30 ml
 
 
   if(time >= time_end2)
   {

digitalWrite(TEQUILA_PIN,HIGH);//60 ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;




case 21:
  time++;
  if(time >= time_end1){


   
  digitalWrite(LIKIER_PIN,HIGH);//30 ml
 digitalWrite(TEQUILA_PIN,HIGH);//30 ml
 
 
   if(time >= time_end2)
   {

 digitalWrite(RUM_PIN,HIGH);//60 ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;







  case 22:
  time++;
  if(time >= time_end1){


   
  
  digitalWrite(RUM_PIN,HIGH);//30 ml
  digitalWrite(LIKIER_PIN,HIGH);//30 ml
 
 
   if(time >= time_end2)
   {

 digitalWrite(VODKA_PIN,HIGH);//60 ml
time = 0;
      time_end1=0;
      time_end2=0;
      state = 0;
   }
  }
  break;








  case 222:
  time++;
  if(time >= time_test){
    digitalWrite(VODKA_PIN,HIGH);
    time = 0;
    time_test = 0;
    state = 0;
  }
  break;


    case 117: // Wlaczenie rumu

  digitalWrite(RUM_PIN,LOW);
 
    break;
     case 118: // Wlaczenie rumu

  digitalWrite(RUM_PIN,HIGH);
 
    break;

case 120: // Wlaczenie rumu

  digitalWrite(VODKA_PIN,LOW);
 
    break;
     case 119: // Wlaczenie rumu

  digitalWrite(VODKA_PIN,HIGH);
  
 
    break;

    case 122: // Wlaczenie rumu

  digitalWrite(LIKIER_PIN,LOW);
 
    break;
     case 121: // Wlaczenie rumu

  digitalWrite(LIKIER_PIN,HIGH);
  
 
    break;



    case 124: // Wlaczenie rumu

  digitalWrite(TEQUILA_PIN,LOW);
 
    break;
     case 123: // Wlaczenie rumu

  digitalWrite(TEQUILA_PIN,HIGH);
  
 
    break;

  // case 115: // Wlaczenie lodowki

  // digitalWrite(Lodowka,LOW);
  // state = 0;


  // break;
  //   case 116: // Wlaczenie lodowki

  // digitalWrite(Lodowka,HIGH);
  // state = 0;


  // break;
}

}
 switch (state)
 {
    case 0:
  digitalWrite(VODKA_PIN,HIGH);
  digitalWrite(RUM_PIN,HIGH);
  digitalWrite(LIKIER_PIN,HIGH);
  digitalWrite(TEQUILA_PIN,HIGH);

  break;
case 115: // Wlaczenie lodowki

  digitalWrite(Lodowka,LOW);
  state = 0;


  break;
    case 116: // Wlaczenie lodowki

  digitalWrite(Lodowka,HIGH);
  state = 0;
    break;
case 128: // Wlaczenie lodowki

  digitalWrite(AKUMULATORKURWATEN,HIGH);
  digitalWrite(Zasilacz,LOW);
  
    break;
    case 129: // Wlaczenie lodowki
 digitalWrite(AKUMULATORKURWATEN,LOW);
  digitalWrite(Zasilacz,HIGH);
    break;




 }


}