/*
 * Kod źródłowy do projektu "Detektor składowych RGB".
 * Wersja 19.01.2024
 * Autorzy: Radosław Mierzwa 263675, Pavlo Kostushevych 251190, Mateusz Gwioździk 263658
 * Opis podłączenia układu:
 * A1 - Arduino UNO rev3
 * U1 - TCS300
 * DS1 - LCD
 * D1 - dioda RGB
 * +5V A1_VIN
 * A1_3V3 U1_VDD
 * A1_A0 DS1_E
 * A1_A1 DS1_D5
 * A1_A2 DS1_D6
 * A1_D2 DS1_RS
 * A1_D3 D1_B
 * A1_D4 DS1_D4
 * A1_D5 D1_G
 * A1_D6 D1_R
 * A1_D7 DS1_D7
 * A1_D8 U1_OUT
 * A1_D9 U1_S0
 * A1_D10 U1_S1
 * A1_D11 U1_S2
 * A1_D12 U1_S3
 * A1_D12 U1_OE
 * A1_GND GND
 */

// Deklaracja pinów podłączonych do LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, A0, 4, A1, A2, 7);

// Deklaracja pinów podłączonych do czujnika TCS3200
#define S0 9
#define S1 10
#define S2 11
#define S3 12
#define OUT 8
#define LED 13

// Deklaracja pinów podłączonych do diody RGB
#define RED 6
#define GREEN 5
#define BLUE 3

// Deklaracja zmiennych użytych w projekcie
long int HALF_PERIOD = 0;
int RED_VALUE = 0;
int GREEN_VALUE = 0;
int BLUE_VALUE = 0;

int RED_LED_VALUE = 0;
int GREEN_LED_VALUE = 0;
int BLUE_LED_VALUE = 0;
String COLOR_NAME;

void setup() {
  // Deklaracja trybu działania pinów podłączanych do czujnika
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(OUT, INPUT);

  // Inicjalizacja wyświetlacza oraz podanie jego wymiarów
  lcd.begin(16, 2);

  // Ustawienie skalowania częstotliwości sygnału wyjściowego z czujnika na 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Zapalenie diod umieszczonych w module z czujnikiem
  digitalWrite(LED, HIGH);

  // Rozpoczęcie przesyłania szeregowego z prędkością bitową 9600 bps
  Serial.begin(9600);
}

void loop() {
  // ROZPOCZĘCIE POMIARÓW SKŁADOWEJ CZERWONEJ
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  HALF_PERIOD = pulseIn(OUT, LOW);
  RED_VALUE = 255 - HALF_PERIOD;
  if (RED_VALUE < 0) RED_VALUE = 0;
  Serial.print("R = ");
  Serial.print(RED_VALUE);
  Serial.print(" ");
  delay(100);

  // ROZPOCZĘCIE POMIARÓW SKŁADOWEJ ZIELONEJ
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  HALF_PERIOD = pulseIn(OUT, LOW);
  GREEN_VALUE = 255 - HALF_PERIOD;
  if (GREEN_VALUE < 0) GREEN_VALUE = 0;
  Serial.print("G = ");
  Serial.print(GREEN_VALUE);
  Serial.print(" ");
  delay(100);

  // ROZPOCZĘCIE POMIARÓW SKŁADOWEJ NIEBIESKIEJ
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  HALF_PERIOD = pulseIn(OUT, LOW);
  BLUE_VALUE = 255 - HALF_PERIOD;
  if (BLUE_VALUE < 0) BLUE_VALUE = 0;
  Serial.print("B = ");
  Serial.println(BLUE_VALUE);
  delay(100);

  // Przypisanie wartości do LED
  RED_LED_VALUE = RED_VALUE;
  GREEN_LED_VALUE = GREEN_VALUE;
  BLUE_LED_VALUE = BLUE_VALUE;

  // Rozpoznawanie koloru
  if (RED_VALUE < 30 && GREEN_VALUE < 30 && BLUE_VALUE < 30) {
    COLOR_NAME = "Black";
  } else if (RED_VALUE > 165 && GREEN_VALUE > 190 && BLUE_VALUE > 190) {
    COLOR_NAME = "White";
  } else if (RED_VALUE < 20 && GREEN_VALUE > 130 && GREEN_VALUE < 180 && BLUE_VALUE > 180 && BLUE_VALUE < 255) {
    COLOR_NAME = "Blue";
  } else if (RED_VALUE > 45 && RED_VALUE < 160 && GREEN_VALUE > 170 && GREEN_VALUE < 210 && BLUE_VALUE > 195 && BLUE_VALUE < 225) {
    COLOR_NAME = "Light Blue";
  } else if (RED_VALUE > 140 && RED_VALUE < 225 && GREEN_VALUE < 20 && BLUE_VALUE < 28) {
    COLOR_NAME = "Red";
    BLUE_LED_VALUE = 0;
  } else if (RED_VALUE > 160 && RED_VALUE < 205 && GREEN_VALUE > 60 && GREEN_VALUE < 115 && BLUE_VALUE > 25 && BLUE_VALUE < 90) {
    COLOR_NAME = "Orange";
    RED_LED_VALUE = 160;
    GREEN_LED_VALUE = 37;
    BLUE_LED_VALUE = 0;
  } else if (RED_VALUE > 40 && RED_VALUE < 90 && GREEN_VALUE < 25 && BLUE_VALUE < 20) {
    COLOR_NAME = "Brown";
    RED_LED_VALUE = 70;
    GREEN_LED_VALUE = 12;
    BLUE_LED_VALUE = 0;
  } else if (RED_VALUE < 50 && GREEN_VALUE > 130 && GREEN_VALUE < 170 && BLUE_VALUE > 60 && BLUE_VALUE < 115) {
    COLOR_NAME = "Green";
    RED_LED_VALUE = 0;
    GREEN_LED_VALUE = 75;
    BLUE_LED_VALUE = 0;
  } else if (RED_VALUE > 120 && RED_VALUE < 170 && GREEN_VALUE > 170 && GREEN_VALUE < 210 && BLUE_VALUE > 100 && BLUE_VALUE < 150) {
    COLOR_NAME = "Light Green";
    RED_LED_VALUE = 54;
    GREEN_LED_VALUE = 130;
    BLUE_LED_VALUE = 18;
  } else if (RED_VALUE > 175 && RED_VALUE < 205 && GREEN_VALUE > 120 && GREEN_VALUE < 165 && BLUE_VALUE > 160 && BLUE_VALUE < 200) {
    COLOR_NAME = "Pink";
    RED_LED_VALUE = 148;
    GREEN_LED_VALUE = 7;
    BLUE_LED_VALUE = 50;
  } else if (RED_VALUE > 180 && RED_VALUE < 220 && GREEN_VALUE > 180 && GREEN_VALUE < 220 && BLUE_VALUE > 100 && BLUE_VALUE < 140) {
    COLOR_NAME = "Yellow";
    RED_LED_VALUE = 66;
    GREEN_LED_VALUE = 55;
    BLUE_LED_VALUE = 0;
  } else {
    COLOR_NAME = "Different Color";
  }

  // Ustawienie wartości pinów podłączonych do diody RGB
  analogWrite(RED, 255 - RED_LED_VALUE);
  analogWrite(GREEN, 255 - GREEN_LED_VALUE);
  analogWrite(BLUE, 255 - BLUE_LED_VALUE);

  // Wyświetlenie wartości składowych RGB oraz nazwy koloru
  lcd.setCursor(0, 0);
  lcd.print("R=");
  lcd.print(RED_VALUE);
  lcd.print("G=");
  lcd.print(GREEN_VALUE);
  lcd.print("B=");
  lcd.print(BLUE_VALUE);
  lcd.setCursor(0, 1);
  lcd.print(COLOR_NAME);

  delay(300);
  lcd.clear();
}
