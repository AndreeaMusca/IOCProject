#include "NoteConstants.h"
#include "MelodyPlayer.h"
#include <LiquidCrystal.h>
#include <TH02_dev.h>
#include <Stepper.h>
#include "PinConstants.h"

TH02_dev TH02;
MelodyPlayer player(BUZZER);
Stepper stepper(NUMBER_OF_STEPS_PER_REV, A, B, C, D);
int speed;
int step;
int maxT;
int minT;
int maxH;
int minH;
int buzzerTone;

void readTemperatureAndHumidity() {
  float temperature = TH02.ReadTemperature();
  float humidity = TH02.ReadHumidity();

  Serial.print("T:");
  Serial.print(temperature);
  Serial.print(",H:");
  Serial.println(humidity);
}
 
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

void moveStepper(){
  stepper.setSpeed(speed);
  stepper.step(step);
}

void changeBuzzerTone(int option)
{
  switch(option){
    case 1:
      buzzerTone =  NOTE_F5;
      break;
    case 2:
      buzzerTone =  NOTE_E6;
      break;
    case 3:
      buzzerTone =  NOTE_A4;
      break;
  }
}


// void processSerialData() {
//   if (Serial.available() > 0) {
//     String input = Serial.readStringUntil('\n'); 

//     if (input.length() >= 0) {
//       String command = input.substring(0, input.indexOf(':')); // Get the command type
//       int value = input.substring(input.indexOf(':') + 1).toInt(); // Extract the value

//       if (command.equals("StepperSpeed")) {
//         changeStepperSpeed(value);
//       } else if (command.equals("BuzzerTone")) {
//         buzzerTone = value;
//       } else if (command.equals("MaxTemperature")) {
//         maxT = value;
//       } else if (command.equals("MinTemperature")) {
//         minT = value;
//       } else if (command.equals("MaxHumidity")) {
//         maxH = value;
//       } else if (command.equals("MinHumidity")) {
//         minH = value;
//       } else {
//         // Command not recognized
//       }
//     }
//   }
// }


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
        case 'M': //MaxT
          maxT = value;
          break;
        case 'm': //MinT
          minT = value;
          break;
        case 'H': //MinT
          maxH = value;
          break;
        case 'h': //MinT
          minH = value;
          break;
        default:
          break;
      }
    }
  }
}


void checkTemperatureAndHumidity() {
  float temper = TH02.ReadTemperature();
  float humidity = TH02.ReadHumidity();
  
  if (temper > maxT) {
    tone(BUZZER, buzzerTone);
    digitalWrite(RED_LED, HIGH);
    //moveStepper();
  } else if ((temper < minT) || (humidity > maxH) || (humidity < minH)) {
    tone(BUZZER, buzzerTone);
    digitalWrite(RED_LED, HIGH);
    delay(500);
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
  //player.play();
  speed = 10;
  step = -NUMBER_OF_STEPS_PER_REV;
  maxT = 27;
  minT = 12;
  maxH = 70;
  minH = 10;
  buzzerTone = NOTE_C5;
}

void loop() {
  processSerialData();
  readTemperatureAndHumidity();
  checkTemperatureAndHumidity();
  // Serial.print(maxT);
  // delay(2000);
}