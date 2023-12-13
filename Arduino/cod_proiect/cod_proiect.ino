#include "NoteConstants.h"
#include "MelodyPlayer.h"
#include <LiquidCrystal.h>
#include <TH02_dev.h>


#define A 4
#define B 7
#define C 12
#define D 13
#define NUMBER_OF_STEPS_PER_REV 512


TH02_dev TH02;
LiquidCrystal lcd(0, 1, 8, 9, 10, 11);

int RED_LED = 2;
int GREEN_LED = 3;
int buzzer = 5;

MelodyPlayer player(buzzer);  // Instantiate MelodyPlayer with buzzer pin

//...............................................................................................

void swichLed(){
  if (Serial.available() > 0) {
    char input = Serial.read();

    if (input == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
    }

    if (input == '0') {
      digitalWrite(LED_BUILTIN, LOW);
    }

    delay(100);
  }
}

void readTemperatureAndHumidity() {
  float temperature = TH02.ReadTemperature();
  float humidity = TH02.ReadHumidity();

  // Display temperature and humidity on the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.print(temper);

  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.print(humidity);

}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  TH02.begin();
}

void loop() {
  swichLed();
  readTemperatureAndHumidity();
  delay(200);
}
