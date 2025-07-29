//Pełen projekt wersja na Maker Uno
// Na początku kodu tworzyny definicje pinów, aby w razie potrzeby
// zminy numeru pinu dla któregokolwiek komponentu wystarczyło zmienić
///w kodzie
// adres pinu tylko w jednym miejscu, jest to duża oszczędność czasu.
// Sensory
#define PIN_sensor_gleba A0
#define PIN_fotorezystor A1
#define PIN_echo 8
#define PIN_trig 9
SS
// Motory
#define PIN_pompa 13
// Oświtlenie - PWM
// Komunikacja
#define PIN_zielony 12
#define PIN_zolty 11
#define PIN_czerwony 10
// Ledy
#define PIN_led1 7
#define PIN_led2 6
#define PIN_led3 5
const int liczbaPomiarow = 5; // Liczba pomiarów do uśrednienia
const long maksymalnyZakres = 400; // Maksymalny zakres czujnika w cm
const long minimalnyZakres = 2; // Minimalny zakres czujnika w cm
// void setup() jest to pętla, która wykonywana jest tylko raz,
// jako pierwsza po uruchomieniu, zrestartowaniu programu.
void setup() {
Serial.begin(9600);
//Nastawy pinów
// Sensory - INPUT
pinMode(PIN_sensor_gleba, INPUT);
pinMode(PIN_fotorezystor, INPUT);
pinMode(PIN_echo, INPUT);
pinMode(PIN_trig, OUTPUT);
// Motory - OUTPUT
pinMode(PIN_pompa, OUTPUT);
// Oświtlenie - OUTPUT
// Komunikacja
pinMode(PIN_zielony, OUTPUT);
pinMode(PIN_zolty, OUTPUT);
pinMode(PIN_czerwony, OUTPUT);
// Ledy
pinMode(PIN_led1, OUTPUT);
pinMode(PIN_led2, OUTPUT);
pinMode(PIN_led3, OUTPUT);
// Definiujemy diody na stan nicki (LOW) w pętli void, żeby mieć pewność,
// że po uruchomieniu programy diody będą zgaszone.
// Komunikacja
digitalWrite(PIN_zielony, LOW);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, LOW);
14
// Ledy
digitalWrite(PIN_led1, LOW);
digitalWrite(PIN_led2, LOW);
digitalWrite(PIN_led3, LOW);
// Tak samo robimy z komponentami z grupy 'Motory'
digitalWrite(PIN_pompa, LOW);
}
// pętla 'void loop' jest powtarzana w kółko, linijka po linijce,
// do momentu wyłączenia lub zresetowania programu.
void loop() {
// tworzymy proste warunki 'If' z jednym warunkiem,
// dla warunku, który nie jest spełniony, polecenie zostaje pominięte.
// Obsługa czujnika wilgotności oraz pompy
// Wyświetlenie danych pomiarowych
// pomiary można odczytać po uruchomieniu serial monitor
// Tools -> Serial Monitor; skrót klawiszowy: Ctrl + Shift + M
Serial.print("Pomiar wilgotności: ");
Serial.println(zmierzGlebe());
// opóżnieni, w tym przypadku dane są zapisywane, co pół sekundy
// 1000 to 1 sekunda
delay(500);
// zdefiniowanie zmiennej, która odpowiada wartości pomiaru gleby
// definiujemy ją jako rezultat programu 'zmierzGlebe()'
int gleba = zmierzGlebe();
// zdefiniowanie zmiennej, która odpowiada wartości pomiaru odległości
// definiujemy ją jako rezultat programu 'zmierzOdleglosc()'
int odleglosc = zmierzOdleglosc();
// Obsługa pompy - definicja warunków (oparte o wartosci 'gleby' i
'odleglosci')
if (gleba > 30 && odleglosc > 0 && odleglosc < 8){
digitalWrite(PIN_pompa, HIGH);
delay(5000);
}
if (gleba <= 30 || odleglosc >= 8 || odleglosc <= 0){
digitalWrite(PIN_pompa, LOW);
delay(5000);
}
// Obsługa systemu oświetlenia
// Fotorezystor jest podłączony do analogowego wejscia, dlatego analog nie
digital
// Definiujemy zmienną 'swiatlo' jako wartosc zczytana przez czujnik
int swiatlo = analogRead(PIN_fotorezystor);
// Wyświetlenie danych pomiarowych
// pomiary można odczytać po uruchomieniu serial monitor
// Tools -> Serial Monitor; skrót klawiszowy: Ctrl + Shift + M
Serial.print("Pomiar światła = ");
Serial.println(swiatlo);
// Obsługa oświetlenia - definicja warunków
if (swiatlo < 120) {
digitalWrite(PIN_led1, HIGH);
digitalWrite(PIN_led2, HIGH);
digitalWrite(PIN_led3, HIGH);
delay(1000);
}
if (swiatlo >= 120) {
digitalWrite(PIN_led1, LOW);
digitalWrite(PIN_led2, LOW);
digitalWrite(PIN_led3, LOW);
15
delay(1000);
}
// Obsługa czujnika odległości
// pomiar wysokości wody w zbiorniku oraz komunikacja z użytkownikiem
// pomiar w cm można odczytać po uruchomieniu serial monitor
// Tools -> Serial Monitor; skrót klawiszowy: Ctrl + Shift + M
// Obsługa komunikacji led - definicja warunków
if (odleglosc > 0) {
Serial.print("Pomiar odległości: ");
Serial.print(odleglosc);
Serial.println(" cm");
} else {
Serial.println("Out of range");
}
// obsøuga diod
// Poziom wody jest BARDZO DOBRY-dioda zielona
if (odleglosc > 0 && odleglosc < 4){
digitalWrite(PIN_zielony, HIGH);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, LOW);
delay(100);
}
// Poziom wody jest OKEJ-dioda zolta
if (odleglosc >= 4 && odleglosc < 8){
digitalWrite(PIN_zielony, LOW);
digitalWrite(PIN_zolty, HIGH);
digitalWrite(PIN_czerwony, LOW);
delay(100);
}
// Poziom wody jest ZLY-dioda czerwona
if (odleglosc >= 8 && odleglosc < 12){
digitalWrite(PIN_zielony, LOW);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, HIGH);
delay(100);
}
// Bøédny pomiar odleglosci wiekszy od glebokosci pudekla lub równa
zeru
if (odleglosc >= 12 || odleglosc <= 0){
digitalWrite(PIN_zielony, HIGH);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, HIGH);
delay(500);
digitalWrite(PIN_zielony, LOW);
digitalWrite(PIN_zolty, HIGH);
digitalWrite(PIN_czerwony, LOW);
delay(500);
digitalWrite(PIN_zielony, HIGH);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, HIGH);
delay(500);
digitalWrite(PIN_zielony, LOW);
digitalWrite(PIN_zolty, HIGH);
digitalWrite(PIN_czerwony, LOW);
delay(500);
digitalWrite(PIN_zielony, HIGH);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, HIGH);
delay(500);
digitalWrite(PIN_zielony, LOW);
digitalWrite(PIN_zolty, HIGH);
digitalWrite(PIN_czerwony, LOW);
delay(500);
digitalWrite(PIN_zielony, HIGH);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, HIGH);
delay(500);
digitalWrite(PIN_zielony, LOW);
digitalWrite(PIN_zolty, HIGH);
16
digitalWrite(PIN_czerwony, LOW);
delay(500);
digitalWrite(PIN_zielony, HIGH);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, HIGH);
delay(500);
digitalWrite(PIN_zielony, LOW);
digitalWrite(PIN_zolty, LOW);
digitalWrite(PIN_czerwony, LOW);
}
}
// Funkcja (program) do zczytywania danych z czujnika gleby
// Nie znajduje się bezpośrednio w pętli, ale jest w niej wywoływana w
// odpowiednim miejscu (linijka 78): "Serial.println(readSensor());"
int zmierzGlebe() {
int sensorValue = analogRead(PIN_sensor_gleba); // Read the analog
value from sensor
int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit
data to 8-bit data
analogWrite(3, outputValue); // generate PWM signal
return outputValue; // Return analog moisture value
}
// Funkcja (program) do zczytywania i kalibracji, stabilizacji pomiarów
// odległości, poziomu wody w zbiorniku
// Nie znajduje się bezpośrednio w pętli, ale jest w niej wywoływana w
// odpowiednim miejscu (linijka 81): "int odleglosc = zmierzOdleglosc();"
int zmierzOdleglosc() {
long czas, sumaCzasow = 0;
int dystans, liczbaPoprawnychPomiarow = 0;
for (int i = 0; i < liczbaPomiarow; i++) {
digitalWrite(PIN_trig, LOW);
delayMicroseconds(2);
digitalWrite(PIN_trig, HIGH);
delayMicroseconds(10);
digitalWrite(PIN_trig, LOW);
czas = pulseIn(PIN_echo, HIGH);
if (czas > 0 && czas < maksymalnyZakres * 58) {
sumaCzasow += czas;
liczbaPoprawnychPomiarow++;
}
delay(10); // Krótka przerwa między pomiarami
}
if (liczbaPoprawnychPomiarow > 0) {
czas = sumaCzasow / liczbaPoprawnychPomiarow;
dystans = czas / 58;
if (dystans >= minimalnyZakres && dystans <= maksymalnyZakres) {
return dystans;
}
}
return -1; // Wartość wskazująca na nieprawidłowy odczyt
}