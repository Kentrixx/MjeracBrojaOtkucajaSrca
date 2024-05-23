#define USE_ARDUINO_INTERRUPTS true
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <PulseSensorPlayground.h>
#include <LCD_I2C.h>

LCD_I2C lcd(0x27);

MD_MAX72XX dispGlavni = MD_MAX72XX(MD_MAX72XX::FC16_HW, 5, 7, 6, 4);

const int duzinaListe = 32;
byte podaci[] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
                  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 };                  
byte animacija[] = { 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000,
                     0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000,
                     0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000,
                     0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000};

int melodija[] = { 523, 659, 784, 880, 988 };

const int PULSE_SENSOR_PIN = 0; 
const int THRESHOLD = 500;
int trenutniBPM;
const int duzinaListeBPM = 30;
int listaBPM[duzinaListeBPM];

PulseSensorPlayground pulseSensor; 

const int ledDioda = 12;

const int buzzer = 13;
float duzinaTona = 0;

unsigned long trenutnoVrijeme = 0;

void setup() {
  lcd.begin();
  lcd.backlight();
  dispGlavni.begin();
  pinMode(ledDioda, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pulseSensor.analogInput(PULSE_SENSOR_PIN);
  pulseSensor.setThreshold(THRESHOLD);
  pulseSensor.begin();
  Serial.begin(9600);
  lcd.print("Pokretanje...");
  for (int i = 0; i < duzinaListe; i++) {
    dispGlavni.setColumn(i, animacija[i]);
    if (i < 5) {
      tone(buzzer, melodija[i]);
      delay(250);
    }
    else
    {
      delay(37.037);
    }
    noTone(buzzer);
    pulseSensor.getBeatsPerMinute();
  }
  lcd.clear();
  lcd.print("Postavite prst");
  lcd.setCursor(0, 1);
  lcd.print("na senzor");
  delay(3000);
  dispGlavni.clear();
  lcd.clear();
}

void loop() {
  for (int i = 0; i < duzinaListeBPM; i++) {
    do {
      trenutniBPM = pulseSensor.getBeatsPerMinute();
    } while (trenutniBPM <= 0);
    listaBPM[i] = trenutniBPM;
  }
  trenutniBPM = izracunajProsjecanBPM();

  lcd.begin();
  lcd.setCursor(0, 0);
  lcd.print("Trenutni BPM:");
  lcd.setCursor(0,1);
  lcd.print(trenutniBPM);
  Serial.print("BPM: ");
  Serial.println(trenutniBPM);

  shiftArray(podaci, duzinaListe, 1);
  novaVrijednostUListi(trenutniBPM);
  for (int i = 0; i < duzinaListe; i++) {
    dispGlavni.setColumn(i, podaci[i]);
  }

  duzinaTona = izracunajDuzinuTona(trenutniBPM);
  trenutnoVrijeme = millis();
  digitalWrite(ledDioda, HIGH);
  tone(buzzer, 2000);
  while (millis() - trenutnoVrijeme < 1000) {
    if (millis() - trenutnoVrijeme >= duzinaTona) {
      noTone(buzzer);
      digitalWrite(ledDioda, LOW);
    }
  }
}

void shiftArray(byte arr[], int size, int shiftBy) {
  for (int i = 0; i < shiftBy; i++) {
    for (int j = size - 1; j > 0; j--) {
      arr[j] = arr[j - 1];
    }
  }
}

void novaVrijednostUListi(int ucitanaVrijednost) {

  if (ucitanaVrijednost < 60) {
    podaci[0] = 0b10000000;
  } 
  else if (ucitanaVrijednost >= 60 && ucitanaVrijednost < 70) {
    podaci[0] = 0b01000000;
  } 
  else if (ucitanaVrijednost >= 70 && ucitanaVrijednost < 80) {
    podaci[0] = 0b00100000;
  }  
  else if (ucitanaVrijednost >= 80 && ucitanaVrijednost < 90) {
    podaci[0] = 0b00010000;
  } 
  else if (ucitanaVrijednost >= 90 && ucitanaVrijednost < 100) {
    podaci[0] = 0b00001000;
  } 
  else if (ucitanaVrijednost >= 100 && ucitanaVrijednost < 110) {
    podaci[0] = 0b00000100;
  } 
  else if (ucitanaVrijednost >= 110 && ucitanaVrijednost < 120) {
    podaci[0] = 0b00000010;
  } 
  else if (ucitanaVrijednost >= 120) {
    podaci[0] = 0b00000001;
  }
  else
  {
    podaci[0] = podaci[1];
  }
}

float izracunajDuzinuTona(int ucitanaVrijednost) {
  return (60.0 / (ucitanaVrijednost * 2)) * 1000;
}

int izracunajProsjecanBPM() {
  int suma = 0;

  for (int i = 0; i < duzinaListeBPM; i++) {
    suma += listaBPM[i];
  }

  return suma / duzinaListeBPM;
}
