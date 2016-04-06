
#include "CharacterConfig.h"

CharacterConfig::CharacterConfig(byte pin, byte files) {
  sensorPin = pin;
  numFiles = files;
}

CharacterConfig::CharacterConfig() {
  sensorPin = numFiles = 0;
}

String CharacterConfig::getRandomAudioFile() {
  String trackName="";
  char t[] = "00";
  sprintf(t, "%d", sensorPin);
  trackName = t;
  trackName+="/";
  sprintf(t, "%d", random(numFiles));
  trackName += t;
  trackName+=".mp3";

  return trackName;
}

byte CharacterConfig::getSensorPin() {
  return sensorPin;
}

