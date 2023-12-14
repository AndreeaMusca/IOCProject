#ifndef MELODY_PLAYER_H
#define MELODY_PLAYER_H

#include <Arduino.h>
#include "NoteConstants.h"

class MelodyPlayer {
public:
  MelodyPlayer(int buzzerPin);

  void play();

private:
  int _buzzerPin;
  
};

#endif
