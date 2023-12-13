#include "MelodyPlayer.h"

MelodyPlayer::MelodyPlayer(int buzzerPin) : _buzzerPin(buzzerPin) {
  pinMode(_buzzerPin, OUTPUT);
}

void MelodyPlayer::play(int tempo) {

  
  int melody[] = {
  // We Wish You a Merry Christmas
  NOTE_C5,4, //1
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
   NOTE_F5,2,

};

  int notes = sizeof(melody) / sizeof(melody[0]) / 2;
  int wholenote = (60000 * 4) / tempo;
  int divider, noteDuration;

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }
    // play the note
    tone(_buzzerPin, melody[thisNote], noteDuration * 0.9);
    // Wait before playing the next note
    delay(noteDuration);
    noTone(_buzzerPin);
  }
}
