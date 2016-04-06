#ifndef MP3UTIL_H
#define MP3UTIL_H

#include "Arduino.h"
#include <SFEMP3Shield.h>


class MP3Util {
  public:
    static byte sayNumber(byte number);
    static byte playNumberMP3(byte number);
    static byte playMP3Blocking(char* trackName);
    static byte playMP3(char* trackName);
    static SFEMP3Shield MP3player;
};
#endif
