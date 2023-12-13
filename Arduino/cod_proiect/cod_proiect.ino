#include "NoteConstants.h"
#include "MelodyPlayer.h"
#include <LiquidCrystal.h>
#include <TH02_dev.h>
#include <Stepper.h>

#define A 4
#define B 7
#define C 12
#define D 13
#define NUMBER_OF_STEPS_PER_REV 2038

TH02_dev TH02;
LiquidCrystal lcd(0, 1, 8, 9, 10, 11);

int RED_LED = 2;
int GREEN_LED = 3;
int buzzer = 5;

MelodyPlayer player(buzzer);
Stepper stepper(NUMBER_OF_STEPS_PER_REV, A, B, C, D);

void changeStepperSpeed() {
  int speed = 10;
  if (Serial.available() > 0) {
    char inputSpeed = Serial.read();
    
    if (inputSpeed == '1') {
      speed = 30;
    } else if (inputSpeed == '2') {
      speed = 60;
    } else if (inputSpeed == '3') {
      speed = 90;
    }
  }
  stepper.setSpeed(speed);
    stepper.step(-NUMBER_OF_STEPS_PER_REV);
    delay(1000);
}


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
  changeStepperSpeed();
  
  
}
