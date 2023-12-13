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



void readTemperatureAndHumidity() {
  float temperature = TH02.ReadTemperature();
  float humidity = TH02.ReadHumidity();

  Serial.print("T:"); 
  Serial.print(temperature);
  Serial.print(",H:");
  Serial.println(humidity);

}


void setup() {
  Serial.begin(9600);
  TH02.begin();
}

void loop() {
  readTemperatureAndHumidity();
  delay(2000);
}
