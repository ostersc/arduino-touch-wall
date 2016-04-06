//comment out to disable debug output
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#include "MP3Util.h"

byte MP3Util::sayNumber(byte number) {
  if (number <= 20) {
    //we can just play these files directly
    return playNumberMP3(number);
  } else if (number < 100) {
    // drop off the ones digit, and say the *ty part (e.g. 30 for 39)
    playNumberMP3((number / 10) * 10);
    //now just say the ones digit (e.g. 9 for 39) unless its 0
    if (number % 10 != 0) {
      playNumberMP3(number % 10);
    }
  } else {
    return -1;
  }
}

byte MP3Util::playNumberMP3(byte number) {
  //a storage place for track names
  char trackName[] = "NUMBERS/00.mp3";
  //sprintf(trackName, "track%03d.mp3", trackNo);
  sprintf(trackName, "NUMBERS/%d.mp3", number);
  //play the file
  return playMP3Blocking(trackName);
}

byte MP3Util::playMP3(char* trackName) {
  return MP3player.playMP3(trackName);
}


byte MP3Util::playMP3Blocking(char* trackName) {
  DEBUG_PRINT("About to play: ");
  DEBUG_PRINTLN(trackName);
  if (MP3player.isPlaying()) {
    DEBUG_PRINTLN("MP3 was already playing, so stopping.");
    MP3player.stopTrack();
  }
  byte r = MP3player.playMP3(trackName);
  do {
    //give it a chance to play the file
    DEBUG_PRINTLN("Predelay");
    delay(100);
    DEBUG_PRINTLN("Postdelay");
  } while (MP3player.isPlaying());

  return r;
}
