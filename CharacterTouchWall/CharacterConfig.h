#ifndef CHARACTERCONFIG_H
#define CHARACTERCONFIG_H

#include "Arduino.h"

class CharacterConfig {
  public:
    CharacterConfig();
    CharacterConfig(byte pin, byte numFiles);
    byte getSensorPin();
    String getRandomAudioFile();
  private:
    byte sensorPin;
    byte numFiles;
};
#endif
