/*
   -- Arduino --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.11 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.11.4 or later version;
     - for iOS 1.9.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__SOFTSERIAL
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX 11
#define REMOTEXY_SERIAL_TX 10
#define REMOTEXY_SERIAL_SPEED 9600


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 200 bytes
  { 255,6,0,22,0,193,0,16,31,1,3,3,12,55,8,22,5,26,3,2,
  37,55,12,22,5,26,129,0,11,83,12,4,24,66,117,122,122,101,114,0,
  129,0,33,83,23,4,24,83,116,101,112,112,101,114,32,115,112,101,101,100,
  0,129,0,4,44,7,3,24,116,95,109,105,110,0,129,0,32,44,7,3,
  24,104,95,109,105,110,0,129,0,18,44,7,3,24,116,95,109,97,120,0,
  129,0,47,44,7,3,24,104,95,109,97,120,0,67,4,36,3,20,5,24,
  16,11,67,4,36,12,20,5,24,16,11,129,0,7,4,24,4,24,84,101,
  109,112,101,114,97,116,117,114,101,58,0,129,0,9,12,16,4,24,72,117,
  109,105,100,105,116,121,58,0,4,0,4,21,7,18,2,26,4,0,19,21,
  7,18,2,26,4,0,34,21,7,18,2,26,4,0,48,21,7,18,2,26 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t buzzerTone; // =0 if select position A, =1 if position B, =2 if position C, ... 
  uint8_t step; // =0 if select position A, =1 if position B, =2 if position C, ... 
  int8_t minT; // =0..100 slider position 
  int8_t maxT; // =0..100 slider position 
  int8_t minH; // =0..100 slider position 
  int8_t maxH; // =0..100 slider position 

    // output variables
  char temperatureDisplayed[11];  // string UTF8 end zero 
  char humidityDisplayed[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


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
float temperature;
float humidity;
LiquidCrystal lcd(A0,A1,A2,A3,9,8);
void readTemperatureAndHumidity() {
  temperature = TH02.ReadTemperature();
  humidity = TH02.ReadHumidity();
  dtostrf(temperature, 6, 2, RemoteXY.temperatureDisplayed);
  dtostrf(humidity, 6, 2, RemoteXY.humidityDisplayed);


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

void processSerialData() {
      // Read values from RemoteXY input variables
      uint8_t remoteBuzzerTone = RemoteXY.buzzerTone;
      uint8_t remoteStep = RemoteXY.step;
      int8_t remoteMinT = RemoteXY.minT;
      int8_t remoteMaxT = RemoteXY.maxT;
      int8_t remoteMinH = RemoteXY.minH;
      int8_t remoteMaxH = RemoteXY.maxH;
      
      // Process the values as needed
      changeStepperSpeed(remoteStep);
      changeBuzzerTone(remoteBuzzerTone);
      maxT = remoteMaxT;
      minT = remoteMinT;
      maxH = remoteMaxH;
      minH = remoteMinH;
}


void checkTemperatureAndHumidity() {
  if (temperature > maxT) {
    tone(BUZZER, buzzerTone);
    digitalWrite(RED_LED, HIGH);
    moveStepper();
  } else if ((temperature < minT) || (humidity > maxH) || (humidity < minH)) {
    tone(BUZZER, buzzerTone);
    digitalWrite(RED_LED, HIGH);
    delay(500);
    digitalWrite(RED_LED, LOW);
    delay(200);
  } else {
    noTone(BUZZER);
    digitalWrite(RED_LED, LOW);

    // Palpitarea LED-ului verde
    for (int i = 0; i < 3; ++i) {  // Palpită de 3 ori
      digitalWrite(GREEN_LED, HIGH);
      delay(200);
      digitalWrite(GREEN_LED, LOW);
      delay(200);
    }
    delay(500); // Pauză între palpitări
  }
}




void setup() {
  RemoteXY_Init (); 
  Serial.begin(9600);
  TH02.begin();
  player.play();
  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Merry"); 
  lcd.setCursor(0, 1);
  lcd.print("Christmas!");
  speed = 10;
  step = -NUMBER_OF_STEPS_PER_REV;
  maxT = 27;
  minT = 12;
  maxH = 70;
  minH = 10;
  buzzerTone = NOTE_C5;
}

void loop() {
  RemoteXY_Handler ();
  processSerialData();
  readTemperatureAndHumidity();
  lcd.setCursor(0, 0);
    lcd.print("Temperature:");
    lcd.print(temperature);
  
    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.print(humidity);
    delay(200);
  checkTemperatureAndHumidity();
  lcd.setCursor(0, 0); // Set the cursor to the first column and first row
  lcd.print("A");
 
}