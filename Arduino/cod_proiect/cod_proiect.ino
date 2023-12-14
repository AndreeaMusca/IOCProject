#include "NoteConstants.h"
#include "MelodyPlayer.h"
#include <LiquidCrystal.h>
#include <TH02_dev.h>
#include <Stepper.h>
#include "PinConstants.h"

TH02_dev TH02;
MelodyPlayer player(BUZZER);
Stepper stepper(NUMBER_OF_STEPS_PER_REV, A, B, C, D);
int speed = 10;
int step = -NUMBER_OF_STEPS_PER_REV;
 
void changeStepperSpeed(int param) {
  switch (param) {
    case 1:
      speed = 5;
      step = NUMBER_OF_STEPS_PER_REV;
      break;
    case 2:
      speed = 10;
      step = -NUMBER_OF_STEPS_PER_REV;
      break;
    default:
      speed = 10;
      step = -NUMBER_OF_STEPS_PER_REV;
      break;
  }
}

void moveStepper()
{
  stepper.setSpeed(speed);
  stepper.step(step);
}

void changeBuzzerTone(int tone) {
  // Handle buzzer tone change based on the received command
  // Implement this function as per your buzzer's functionality
}

//The commands should look like "S:1", "S:2", "B:1",....
void processSerialData() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); 

    if (input.length() >= 3) {
      char command = input.charAt(0); // Get the command type
      int value = input.substring(2).toInt(); // Extract the value

      switch (command) {
        case 'S': // Stepper command
          changeStepperSpeed(value);
          break;
        case 'B': // Buzzer command
          changeBuzzerTone(value);
          break;
        default:
          break;
      }
    }
    Serial.flush();
  }
 
}

void readTemperatureAndHumidity() {
  float temperature = TH02.ReadTemperature();
  float humidity = TH02.ReadHumidity();

  Serial.print("T:");
  Serial.print(temperature);
  Serial.print(",H:");
  Serial.println(humidity);
}


void checkTemperatureAndHumidity() {
  float temper = TH02.ReadTemperature();
  float humidity = TH02.ReadHumidity();
  
  if (temper > 27) {
    tone(BUZZER, 500);
    digitalWrite(RED_LED, HIGH);
    moveStepper();
  } else if ((temper < 17) || (humidity > 70) || (humidity < 10)) {
    tone(BUZZER, 500);
    digitalWrite(RED_LED, HIGH);
    delay(300);
    digitalWrite(RED_LED, LOW);
    delay(200);
  } else {
    noTone(BUZZER);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(500);
    digitalWrite(GREEN_LED, LOW);
    delay(200);
  }
}




















void setup() {
  Serial.begin(9600);
  TH02.begin();
  stepper.setSpeed(5);
  //player.play();
}

void loop() {
  //processSerialData();
  readTemperatureAndHumidity();
  //checkTemperatureAndHumidity();
}
