/*Tytul: Sensor Display Integration
Autor: Radosław Mierzwa
Wersja: 1.0
Data: 16 listopada 2024
Cel: Ten kod integruje czujniki i wyświetla ich odczyty na wyświetlaczu e-papierowym. Zbiera dane dotyczące jakości powietrza, w tym pyły zawieszone, temperaturę, wilgotność, ciśnienie oraz indeks VOC. Dane są zapisywane na karcie SD, a urządzenie przechodzi w tryb głębokiego snu, aby oszczędzać energię, gdy nie dokonuje pomiarów.
Wymagania sprzętowe:
- Mikrokontroler ESP32
- Wyświetlacz e-papierowy GxEPD
- Czujnik pyłu zawieszonego PMS5003
- Czujnik środowiskowy BME680
- Czujnik VOC SGP40
- Moduł RTC SD2405
- Moduł karty SD
- Zasilanie i wspierające komponenty
*/

#include <GxEPD.h>
#include <GxGDEM029T94/GxGDEM029T94.h>
#include GxEPD_BitmapExamples
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <GravityRtc.h>  // Biblioteka dla RTC SD2405
#include "DFRobot_SGP40.h"  // Czujnik VOC SGP40
#include "DFRobot_BME680_I2C.h"  // Czujnik BME680
#include "DFRobot_MICS.h"  // Czujnik MICS-4514
#include <SD.h>   
#include "PMS.h"
#include "WiFi.h"
#include "esp_sleep.h"
#include "esp_bt.h"

#define ADC_PIN 34  // Pin ADC, np. GPIO34
#define VOLTAGE_DIVIDER_RATIO 2.0  // np. dzielnik 10 kΩ / 10 kΩ
#define EPD_CS 25
#define EPD_DC 26
#define EPD_RST 27
#define EPD_BUSY 9
#define BUTTON_DECREASE_PIN 12
#define BUTTON_INCREASE_PIN 13
#define BUTTON_OK_PIN 14
#define chipSelect 5 
#define RX_PIN 16  // Pin RX dla PMS5003 (TX z PMS5003)
#define TX_PIN 17  // Pin TX dla PMS5003 (RX z PMS5003)
#define SHDN_PIN 4

// Czujniki i moduły
GxIO_Class io(SPI, EPD_CS, EPD_DC, EPD_RST);
GxEPD_Class display(io, EPD_RST, EPD_BUSY);
GravityRtc rtc;  // Moduł RTC SD2405
DFRobot_SGP40 mySgp40;  // Czujnik VOC SGP40
DFRobot_BME680_I2C bme(0x76);  // Czujnik BME680

// Ustawienia UART dla PMS5003
HardwareSerial pmsSerial(1);
PMS pms(pmsSerial);
PMS::DATA data;
// Zmienne do przechowywania daty i czasu
int year = 2024;
int month = 1;
int day = 1;
int hour = 0;
int minute = 0;
int second = 0;

// Flagi ustawień
bool settingDate = true;  // Flaga do ustawiania daty
bool settingInterval = false;  // Flaga do ustawiania interwału
bool settingStartDate = false;  // Flaga do ustawiania daty startu
bool settingStartDate2 = false;  // Flaga do ustawienia startu drugiego kroku
int step = 0;  // Kroki ustawień: 0 - rok, 1 - miesiąc, itd.
// Flaga w RTC RAM do zapisania konfiguracji
RTC_DATA_ATTR bool isConfigured = false;  

// Zmienna do debounce przycisków
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 20;  // Skrócony czas debounce (20 ms)
RTC_DATA_ATTR unsigned long long interval = 3600000ULL;  // 1 godzina
bool changeDetected = false;
bool isMeasuring = false;
unsigned long measureStartTime = 0;
unsigned long startTime = 0;
unsigned long heatingStartTime = 0;
unsigned long pmsWakeupTime = 0;
bool isPmsWakingUp = false;

// Zmienne do śledzenia pomiarów
unsigned long lastMeasurementTime = 0;  // Czas ostatniego pomiaru
// Interwał pomiaru (10 sekund)
const unsigned long measurementInterval = 10000; 

// Zmienne do BME680
unsigned long bmeStartConvertTime = 0;  // Czas rozpoczęcia konwersji BME680
const unsigned long bmeInterval = 5000; // Czas konwersji co 5 sekund
bool isBmeConverting = false;

// Zmienne do przechowywania wartości pomiarów
float pm1 = 0.0;
float pm25 = 0.0;
float pm10 = 0.0;
float temperature = 0.0;
float humidity = 0.0;
float pressure = 0.0;
uint16_t vocIndex = 0;  // VOC index z SGP40

// Zmienna do przechowywania napięcia baterii
int adcValue = 0;
float voltage = 0.0;

// Definicje stanów dla automatu stanowego pomiarów
enum MeasurementState {
  WAITING,          // Oczekiwanie na rozpoczęcie cyklu pomiarowego
  WAKING_UP_PMS,    // Wybudzanie PMS5003
  WAITING_STABILIZE, // Oczekiwanie 30 sekund na stabilizację
  PERFORMING_MEASUREMENT  // Wykonywanie pomiaru
};
MeasurementState state = WAITING;  // Zmienna stanu
//unsigned long previousMillis = 0;  // Zmienna do śledzenia czasu
//unsigned long stabilizeTime = 30000;  // 30 sekund stabilizacji
//unsigned long long wakeUpTime = 0ULL;
unsigned long long totalBytes = 0ULL;
unsigned long UsedBytes = 0;

// Funkcja zapisująca dane na karcie SD
void zapisDanych() {
  // Inicjalizacja karty SD w funkcji zapisu
  SPI.begin();
  if (!SD.begin(chipSelect)) {
    Serial.println("Błąd inicjalizacji karty SD!");
    return;  // Zakończ funkcję, jeśli karta SD nie działa
  }
  Serial.println("Karta SD zainicjowana.");

  // Pobranie aktualnej daty z RTC
  rtc.read();

  // Odczyt całkowitej i użytej pojemności karty SD
  totalBytes = SD.totalBytes();
  UsedBytes = SD.usedBytes();
  Serial.println("Całe dane: ");
  Serial.println(totalBytes);
  Serial.println("Użyte dane: ");
  Serial.println(UsedBytes);

  // Nazwa folderu do zapisywania danych
  const char* folderName = "/Zanieczyszczenia_powietrza_pomiary";

  // Sprawdzenie, czy folder istnieje, jeśli nie - utwórz
  if (!SD.exists(folderName)) {
    if (SD.mkdir(folderName)) {
      Serial.println("Folder Zanieczyszczenia_powietrza_pomiar utworzony.");
    } else {
      Serial.println("Błąd tworzenia folderu Zanieczyszczenia_powietrza_pomiar!");
      return;  // Zakończ funkcję, jeśli nie udało się utworzyć folderu
    }
  }

  // Tworzenie nazwy pliku na podstawie dnia, miesiąca i roku
  String monthNames[] = {"styczen", "luty", "marzec", "kwiecien", "maj", "czerwiec", 
                         "lipiec", "sierpien", "wrzesien", "pazdziernik", "listopad", "grudzien"};
  String fileName = String(folderName) + "/" + String(rtc.day) + "_" + monthNames[rtc.month - 1] + "_" + String(rtc.year) + ".txt";

  // Sprawdzenie, czy plik istnieje, jeśli nie - tworzymy nowy
  if (!SD.exists(fileName.c_str())) {
    File myFile = SD.open(fileName.c_str(), FILE_WRITE);
    if (myFile) {
      Serial.print("Tworzę plik "); Serial.println(fileName);
      myFile.println("Pomiar z PMS5003 i RTC");  // Dodaj nagłówki lub pierwszą linię
      myFile.close();
      Serial.println("Plik utworzony.");
    } else {
      Serial.println("Błąd tworzenia pliku");
      return;  // Zakończ funkcję, jeśli nie udało się otworzyć pliku
    }
  }

  // Dopisywanie danych do istniejącego lub nowo utworzonego pliku
  File myFile = SD.open(fileName.c_str(), FILE_APPEND);
  if (myFile) {
    // Zapisz aktualną datę i czas z RTC
    myFile.print("Czas: ");
    myFile.print(rtc.year);
    myFile.print('/');
    myFile.print(rtc.month);
    myFile.print('/');
    myFile.print(rtc.day);
    myFile.print(" ");
    myFile.print(rtc.hour);
    myFile.print(':');
    myFile.print(rtc.minute);
    myFile.print(':');
    myFile.print(rtc.second);
    myFile.println();

    // Zapisz dane z czujników
    myFile.print("PM1.0: ");
    myFile.print(pm1);
    myFile.println(" ug/m3");

    myFile.print("PM2.5: ");
    myFile.print(pm25);
    myFile.println(" ug/m3");

    myFile.print("PM10: ");
    myFile.print(pm10);
    myFile.println(" ug/m3");

    myFile.print("Temperatura: ");
    myFile.print(temperature);
    myFile.println(" C");

    myFile.print("Wilgotność: ");
    myFile.print(humidity);
    myFile.println(" %");

    myFile.print("Ciśnienie: ");
    myFile.print(pressure);
    myFile.println(" hPa");

    myFile.print("VOC Index: ");
    myFile.print(vocIndex);
    myFile.println();

    myFile.println("-----------------------");
    myFile.close();  // Zamknij plik po zapisaniu danych
    Serial.println("Dane zapisane na karcie SD.");
  } else {
    Serial.println("Błąd otwarcia pliku");
  }

  SPI.end();
}





// Inicjalizacja w setup
void setup() {
  Serial.begin(115200);  // Rozpoczęcie komunikacji szeregowej
  pmsSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);  // Inicjalizacja portu szeregowego dla PMS5003
  
  pms.passiveMode();  // Ustawienie czujnika PMS5003 w tryb pasywny

  WiFi.disconnect(true);  // Wyłączenie WiFi
  WiFi.mode(WIFI_OFF);  // Tryb WiFi OFF
  esp_bt_controller_disable();  // Wyłączenie Bluetooth

  rtc.setup();  // Inicjalizacja RTC SD2405
  pinMode(SHDN_PIN, OUTPUT);  // Ustawienie pinu SHDN jako wyjścia
// Ustawienie pinu RESET wyświetlacza jako wyjścia
  pinMode(EPD_RST, OUTPUT);  

  analogReadResolution(12);  // Ustawienie 12-bitowej rozdzielczości ADC (0-4095)
  analogSetAttenuation(ADC_11db);  // Ustawienie zakresu napięcia do 3.3V

  // Odblokowanie GPIO4 (SHDN_PIN) po wybudzeniu
  gpio_hold_dis(GPIO_NUM_4);
// Włącz przetwornicę (stan wysoki na SHDN)
  digitalWrite(SHDN_PIN, HIGH);  

  // Inicjalizacja pinów przycisków jako wejścia
  pinMode(BUTTON_INCREASE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DECREASE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OK_PIN, INPUT_PULLUP);
  
  // Inicjalizacja wyświetlacza, tylko jeśli nie ustawiono daty
  if(!isConfigured){
    SPI.begin(); 
    digitalWrite(EPD_RST, HIGH);
    display.init(115200);  // Inicjalizacja wyświetlacza
    display.fillScreen(GxEPD_WHITE);  // Wyczyszczenie ekranu na biało
    display.setRotation(1);  // Ustawienie orientacji poziomej
// Ustawienie koloru tekstu na czarny
    display.setTextColor(GxEPD_BLACK);  
    display.update();
    digitalWrite(EPD_RST, LOW);
    SPI.end(); 
  }
  
  rtc.read();  // Odczyt bieżącego czasu z RTC

  // Inicjalizacja czujnika VOC SGP40
  while (!mySgp40.begin(10000)) {  // Czas na nagrzewanie czujnika SGP40
    Serial.println("Błąd inicjalizacji SGP40.");
    delay(1000);
  }
  Serial.println("Czujnik SGP40 pomyślnie zainicjalizowany.");

  // Inicjalizacja czujnika BME680
  uint8_t wynik = 1;
  while (wynik != 0) {
    wynik = bme.begin();
    if (wynik != 0) {
      Serial.println("Błąd inicjalizacji BME680");
      delay(2000);
    }
  }
  Serial.println("Czujnik BME680 pomyślnie zainicjalizowany.");

  if(!isConfigured) {
    updateDisplay2();  // Aktualizacja wyświetlacza podczas ustawiania
  } else {
    startTime = millis();  // Rozpoczęcie odliczania czasu
  }
}

// Funkcja odpowiedzialna za aktualizację ekranu - wyświetlanie bieżących pomiarów
void updateDisplay() {
  rtc.read();  // Odczytaj bieżący czas z RTC przed aktualizacją wyświetlacza
  digitalWrite(EPD_RST, HIGH);  // Ustawienie pinu RESET na wysoki stan
  SPI.begin();
  display.init(115200);  // Inicjalizacja wyświetlacza
  display.fillScreen(GxEPD_WHITE);  // Wyczyszczenie ekranu na biało
  display.setRotation(1);  // Ustawienie orientacji poziomej
  display.setTextColor(GxEPD_BLACK);  // Ustawienie koloru tekstu na czarny
  display.update();

  // Wyświetlanie daty i czasu na ekranie
  display.setTextSize(1); // Zmniejszenie czcionki
  display.fillScreen(GxEPD_WHITE);  // Czyszczenie ekranu
  display.setCursor(10, 0);
  display.print("Ostatnia aktualizacja: ");
  display.print(rtc.year);
  display.print('/');
  display.print(rtc.month);
  display.print('/');
  display.print(rtc.day);
  display.print("  ");
  display.print(rtc.hour);
  display.print(':' );
  display.print(rtc.minute);
  display.print(':' );
  display.print(rtc.second);

  // Wyświetlanie wartości pomiarów
  display.setCursor(10, 20);
  display.print("PM 1.0: ");
  display.print(pm1);
  display.print(" PM 2.5: ");
  display.print(pm25);
  display.print(" PM 10: ");
  display.print(pm10);
  display.setCursor(10, 40);
  display.print("Temperatura: ");
  display.print(temperature);
  display.print(" C");
  display.setCursor(10, 50);
  display.print("Wilgotnosc: ");
  display.print(humidity);
  display.print(" %");
  display.setCursor(10, 60);
  display.print("Cisnienie: ");
  display.print(pressure);
  display.print(" hPa");
  display.setCursor(10, 70);
  display.print("VOC Indeks: ");
  display.print(vocIndex);
  display.setCursor(10, 90);
  display.print("Pomiar co: ");
  display.print(interval / 3600000);  // Wypisanie interwału w godzinach
  display.print(" godz.");
  display.setCursor(10, 100);
  display.print("Napiecie baterii: ");
  display.print(voltage);  // Wypisanie napięcia baterii
  display.print(" V");
  display.setCursor(10, 110);
  display.print("Pozostala pojemnosc karty SD: ");
  display.print((((totalBytes-UsedBytes)/1024)/1024));  // Wypisanie dostępnej pamięci
  display.print(" MB");
  display.update();  // Aktualizacja wyświetlacza
  digitalWrite(EPD_RST, LOW);  // Wyłącz wyświetlacz, ustawiając RESET w stan niski
  SPI.end(); 
}

// Funkcja odpowiedzialna za aktualizację ekranu z komunikatem o naładowaniu baterii
void updateDisplay3() {
  digitalWrite(EPD_RST, HIGH);  // Włącz wyświetlacz
  SPI.begin();
  display.init(115200);  // Inicjalizacja wyświetlacza
  display.fillScreen(GxEPD_WHITE);  // Czyszczenie ekranu
  display.setRotation(1);  // Ustawienie orientacji poziomej
  display.setTextColor(GxEPD_BLACK);  // Ustawienie koloru tekstu na czarny
  display.update();

  display.setTextSize(2);  // Zwiększenie czcionki
  display.setCursor(10, 20);
  display.print("NALADUJ BATRIE NAPIECIE");
  display.setCursor(110, 50);
  display.print(voltage);
  display.print(" V");
  display.setCursor(60, 80);
  display.print("TRYB DEEP SLEEP  ");
  display.setCursor(10, 110);
  display.print("WYBUDZ (PRZYCISKIEM +)  ");
  display.update();  // Aktualizacja wyświetlacza
  digitalWrite(EPD_RST, LOW);  // Wyłącz wyświetlacz, ustawiając RESET w stan niski
  SPI.end(); 
}

// Funkcja dokonywania pomiaru - pobieranie danych z czujników
void performMeasurement() {
  unsigned long currentMillis = millis();  // Pobranie bieżącego czasu

  // ----- Pomiar BME680 co 5 sekund -----
  if (!isBmeConverting && currentMillis - bmeStartConvertTime >= bmeInterval) {
    bme.startConvert();  // Rozpocznij konwersję
    bmeStartConvertTime = currentMillis;  // Zapisz czas rozpoczęcia
    isBmeConverting = true;  // Flaga, że trwa konwersja
  }

  // Sprawdź, czy minęło 1000 ms na zakończenie konwersji BME680
  if (isBmeConverting && currentMillis - bmeStartConvertTime >= 1000) {
    bme.update();  // Zaktualizuj dane z czujnika
    temperature = bme.readTemperature() / 100;
    humidity = bme.readHumidity() / 1000;
    pressure = bme.readPressure() / 100;
    Serial.print("Temperatura: ");
    Serial.println(temperature);
    Serial.print("Wilgotność: ");
    Serial.println(humidity);
    Serial.print("Ciśnienie: ");
    Serial.println(pressure);

    isBmeConverting = false;  // Konwersja zakończona

  }

  adcValue = analogRead(ADC_PIN);  // Odczyt wartości ADC
  voltage = (((adcValue / 4095.0) * 3.3) * VOLTAGE_DIVIDER_RATIO) + 0.3;

  Serial.print("ADC Value: ");
  Serial.print(adcValue);
  Serial.print(", Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  if (!isPmsWakingUp) {
    pms.wakeUp();  // Wybudzenie PMS5003
    Serial.println("Wybudzam PMS5003. Oczekiwanie na stabilizację.");
    isPmsWakingUp = true;
  }

  if (currentMillis - lastMeasurementTime >= measurementInterval) {
    PMS::DATA data;
    pms.requestRead();  // Żądanie odczytu w trybie pasywnym

    if (pms.readUntil(data)) {
      pm1 = data.PM_AE_UG_1_0;
      pm25 = data.PM_AE_UG_2_5;
      pm10 = data.PM_AE_UG_10_0;

      // Wyświetlanie wyników PMS5003
      Serial.print("PM1.0: ");
      Serial.print(pm1);
      Serial.println(" ug/m3");
      Serial.print("PM2.5: ");
      Serial.print(pm25);
      Serial.println(" ug/m3");
      Serial.print("PM10: ");
      Serial.print(pm10);
      Serial.println(" ug/m3");

      pms.sleep();  // Po pomiarze usypiamy PMS5003
      Serial.println("PMS5003 idzie spać.");
      isPmsWakingUp = false;
      lastMeasurementTime = currentMillis;  // Resetowanie licznika czasu
    }
  }

/*
Title: Sensor Display Integration - Part 2
Author: Radosław Mierzwa
Version: 1.0
Date: 16 listopada 2024
*/
// ----- Odczyt VOC Index -----
vocIndex = mySgp40.getVoclndex();
Serial.print("VOC Index: ");
Serial.println(vocIndex);
}

// Funkcja główna loop - główna pętla programu
void loop() {
  // Pomiar napięcia baterii
  adcValue = analogRead(ADC_PIN);  // Odczyt wartości ADC
  voltage = (((adcValue / 4095.0) * 3.3) * VOLTAGE_DIVIDER_RATIO)+0.3;
  delay(100);

  // Sprawdzenie napięcia baterii - różne scenariusze w zależności od napięcia
  if(voltage > 3.35) {
    configureSettings();  // Wywołaj konfigurację w pętli loop

    if(isConfigured) {  // Jeśli ustawienia zostały zakończone
      unsigned long currentTime = millis();
      if ((currentTime - startTime >= 60000 )) {  // Sprawdzenie, czy minęło 60 sekund
        zapisDanych();  // Zapisz dane na kartę SD
        delay(200);
        pms.sleep();
        Serial.println("1 min minęła, przechodzenie w deep sleep.");

        Serial.print("Przechodzenie w deep sleep na ");
        Serial.print(interval / 3600000ULL);
        Serial.println(" godziny/e.");
        if (temperature >= 0) {
          updateDisplay();  // Aktualizacja wyświetlacza
        }
        
        digitalWrite(SHDN_PIN, LOW);  // Wyłącz przetwornicę
        Serial.println("Przetwornica wyłączona, przechodzę w tryb deep sleep...");
        gpio_hold_en(GPIO_NUM_4);  // Zatrzymanie aktualnego stanu na GPIO4 (SHDN_PIN)
        esp_sleep_enable_timer_wakeup(interval * 1000);  // Ustawienie wybudzania na podstawie czasu
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);  // Przycisk DECREASE_PIN (-) wybudza
        delay(1000);
        esp_deep_sleep_start();  // Przejście w tryb deep sleep
      }
    }

    // Rozpoczęcie pomiaru, jeśli nie jest już w trakcie
    if (!isMeasuring) {
      measureStartTime = millis();
      isMeasuring = true;
      performMeasurement();  // Dokonanie pomiaru
    }

    // Aktualizacja ekranu po zakończeniu pomiaru
    if ((millis() - measureStartTime) >= 10000) {  
      isMeasuring = false;
    }
  } else {
    bme.update();  // Zaktualizuj dane z czujnika
    temperature = bme.readTemperature() / 100;
    Serial.println("temp wynosi: ");
    Serial.println(temperature);

    if (temperature >= 0) {
      updateDisplay3();  // Aktualizacja wyświetlacza dla komunikatu o baterii
    }

    delay(1000);
    digitalWrite(SHDN_PIN, LOW);  // Wyłącz przetwornicę
    gpio_hold_en(GPIO_NUM_4);  // Zatrzymanie aktualnego stanu na GPIO4 (SHDN_PIN)
// Przycisk DECREASE_PIN wybudza
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);  
    esp_deep_sleep_start();
  }
}

// Funkcja do aktualizacji wyświetlacza przy ustawianiu daty
void updateDisplay2() {
  digitalWrite(EPD_RST, HIGH);
  SPI.begin();
  display.setTextSize(2); // Zwiększenie czcionki
  display.fillScreen(GxEPD_WHITE);  // Czyszczenie ekranu
  display.setCursor(10, 20);
  delay(100);

  // Wyświetlanie odpowiedniego kroku ustawień daty
  if (settingDate) {
    switch (step) {
      case 0:
        display.print("Podaj rok: ");
        display.print(year);
        break;
      case 1:
        display.print("Podaj miesiac: ");
        display.print(month);
        break;
      case 2:
        display.print("Podaj dzien: ");
        display.print(day);
        break;
      case 3:
        display.print("Podaj godzine: ");
        display.print(hour);
        break;
      case 4:
        display.print("Podaj minuty: ");
        display.print(minute);
        break;
      case 5:
        display.print("Podaj sekundy: ");
        display.print(second);
        break;
    }
  } else if (settingInterval) {
    display.setCursor(65, 20);
    display.print("Ustaw interwal: ");
    display.setCursor(100, 50);
// Wyświetlanie interwału w godzinach
    display.print(interval / 3600000);  
    display.print(" godz.");
  } else if (settingStartDate) {
    display.setTextSize(1); // Zmniejszenie czcionki
    display.setCursor(10, 10);
    display.print("Date oraz interwal ustawiono pomyslnie: ");
    display.setCursor(10, 25);
    display.print("Krotka instrukcja: ");
    display.setCursor(10, 40);
    display.print("-Aby rozpoczac pomiar wcisnij OK ");
    display.setCursor(10, 55);
    display.print("-Pomiar wyswietli sie za 1 min.");
    display.setCursor(10, 75);
    display.print("-Wylacz i wlacz zasilanie aby zmienic parametry");
    display.setCursor(10, 95);
    display.print("-Dane sa zapisywane na karcie SD");
    display.setCursor(10, 115);
    display.print("-Urzadzenie mozna ladowac");
    settingStartDate2 = true;
  } else if (settingStartDate2) {
    display.setTextSize(2); // Zmniejszenie czcionki
    display.setCursor(10, 20);
    display.print("Pomiar wyswietli sie za: ");
    display.setCursor(100, 50);
    display.print("1 min.");
    display.setCursor(10, 80);
    display.print("jezeli temp. > 0 C");
  }

  display.update();
  Serial.println("Ekran zaktualizowany.");
  digitalWrite(EPD_RST, LOW);
  SPI.end();
}

// Funkcja do sterowania przyciskami - obsługa ustawień daty i interwału
void handleButtons() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastButtonPress >= debounceDelay) {
    if (digitalRead(BUTTON_INCREASE_PIN) == LOW) {
      lastButtonPress = currentMillis;
      Serial.println("Przycisk INCREASE naciśnięty.");
      if (settingDate) {
        // Zwiększanie odpowiedniego parametru daty
        if (step == 0) year++;
        else if (step == 1) {   // Miesiąc
          month++;
          if (month > 12) month = 1;
        } else if (step == 2) { // Dzień
          day++;
          if (day > 31) day = 1;
        } else if (step == 3) { // Godzina
          hour++;
          if (hour > 23) hour = 0;
        } else if (step == 4) { // Minuta
          minute++;
          if (minute > 59) minute = 0;
        } else if (step == 5) { // Sekunda
          second++;
          if (second > 59) second = 0;
        }
      } else if (settingInterval) {
         interval += 3600000ULL;  // Zwiększenie o godzinę
      }
      updateDisplay2();
    }
    if (digitalRead(BUTTON_DECREASE_PIN) == LOW) {
      lastButtonPress = currentMillis;
      Serial.println("Przycisk DECREASE naciśnięty.");
      if (settingDate) {
        // Zmniejszanie odpowiedniego parametru daty
        if (step == 0) year--;
        else if (step == 1) {   // Miesiąc
          month--;
          if (month < 1) month = 12;
        } else if (step == 2) { // Dzień
          day--;
          if (day < 1) day = 31;
        } else if (step == 3) { // Godzina
          hour--;
          if (hour < 0) hour = 23;
        } else if (step == 4) { // Minuta
          minute--;
          if (minute < 0) minute = 59;
        } else if (step == 5) { // Sekunda
          second--;
          if (second < 0) second = 59;
        }
      } else if (settingInterval) {
        interval = max(3600000ULL, interval - 3600000ULL);
        Serial.println("Interwał wynosi :");
        Serial.println(interval);
      }
      updateDisplay2();
    }

    if (digitalRead(BUTTON_OK_PIN) == LOW) {
      lastButtonPress = currentMillis;
      Serial.println("Przycisk OK naciśnięty.");
      if (settingDate) {
        step++;
        if (step > 5) {
          settingDate = false;
          settingInterval = true;
        }
      } else if (settingInterval) {
        settingInterval = false;
        settingStartDate = true;
      } else if (settingStartDate) {
        settingStartDate = false;
        rtc.adjustRtc(year, month, day, 0, hour, minute, second);  // Zapisz datę i czas do RTC
        Serial.println("Data i czas zapisane.");
        isConfigured = true;
        startTime = millis();
      }
      updateDisplay2();
    }
  }
}

// Funkcja do konfiguracji ustawień - obsługa konfiguracji początkowej
void configureSettings() {
  while (!isConfigured) {  // Pętla do momentu zakończenia konfiguracji
    handleButtons();  // Obsługa przycisków i zmiany ustawień
  }
}

