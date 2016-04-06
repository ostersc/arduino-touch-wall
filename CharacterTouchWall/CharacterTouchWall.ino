
//comment out to disable debug output
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

/**
  SD Card Layout:
   CONFIG/
     MODE[0|1]
   FX/
     correct.mp3
     incorrect.mp3
     timeout.mp3
     round.mp3
     error.mp3
   NUMBERS/
        (0-19,20-90).mp3
   SOUNDS/
     0..11/
       (0..n).mp3
*/

// compiler error handling
#include "Compiler_Errors.h"
#include <MemoryFree.h>

// touch includes
#include <MPR121.h>  // https://github.com/BareConductive/mpr121
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4

// mp3 includes
#include <SPI.h>
#include <SdFat.h>  // https://github.com/greiman/SdFat   https://codebender.cc/library/SdFat
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>

// sd includes
//#include <FSConf.h>

//game stuff
#include "Game.h"
#include "MP3Util.h"

// mp3 variables
SFEMP3Shield MP3player;
SFEMP3Shield MP3Util::MP3player = MP3player;
// sd card instantiation
SdFat sd;
//FSConf config("CONFIG");

//game structure
#define TOUCH_SOUND  true
#define TRIVIA  false
bool mode = TOUCH_SOUND; // which mode to start in

Game game;
CharacterConfig characters[12];

void setup() {
  //init the random number generator
  randomSeed(analogRead(0));

  Serial.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);

#ifdef DEBUG
  while (!Serial) ; {} //wait for the serial monitor when in debug
#endif

  if (!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();

  if (!MPR121.begin(MPR121_ADDR)) DEBUG_PRINTLN("error setting up MPR121");
  MPR121.setInterruptPin(MPR121_INT);

  MPR121.setTouchThreshold(40);
  MPR121.setReleaseThreshold(20);

  byte mp3InitResult = MP3player.begin();
  MP3player.setVolume(10, 10);

  if (mp3InitResult != 0) {
    DEBUG_PRINT("Error code: ");
    DEBUG_PRINT(mp3InitResult);
    DEBUG_PRINTLN(" when trying to start MP3 player.");
  }

  //debug config
  //config.set_debug_stream(&Serial);

  //read configs
  //config.begin(SD_SEL);
  //this is for local scope only
  //FSCONFIG_get_config(config, mode, "MODE");
  //config.getValue(mode, 1, "MODE");

  DEBUG_PRINTLN("Loading rounds.");

  //TODO: if running out of program storage space; replace these macros, they seem inefficient
  //FSCONFIG_get_config(config, rounds, "TRIVIA", "NUM_ROUNDS");
  //game = Game(atoi(rounds));
  game = Game(10);

  DEBUG_PRINT("Star Wars Touch Wall.  Using mode: ");
  DEBUG_PRINTLN(mode);

  initCharacters();

  //  DEBUG_PRINT("freeMemory()=");
  //  DEBUG_PRINTLN(freeMemory());
}


bool initCharacters() {
  //read from the SD card in a pin number folder and populate the audiofiles array from all that are found
  DEBUG_PRINTLN("Loading characters.");
  SdFile file;
  char filename[13];
  byte numFiles = 0;
  sd.chdir("/", true);
  for (byte i = 0; i < 12; i++) {
    numFiles = 0;
    DEBUG_PRINT("Building character: ");
    DEBUG_PRINTLN(i);
    char dir[] = "/00";
    sprintf(dir, "/%d", i);
    sd.chdir(dir, true);
    while (file.openNext(sd.vwd(), O_READ)) {
      if (file.isFile()) {
        file.getFilename(filename);
        DEBUG_PRINTLN(filename);
        //if (filename.endsWith(".mp3")) {
        //hack to strip out the junk mac puts there
        if (filename[0] != '_' && filename[0] != '~' && filename[0] != 'D') {
          numFiles++;
        }

      }
      file.close();
    }
    DEBUG_PRINT("Found file count of: ");
    DEBUG_PRINTLN(numFiles);
    characters[i] = CharacterConfig(i, numFiles);
  }
  sd.chdir("/", true);
}


void loop() {
  //delegate to the appropriate mode handler; a false means the mode should be switched
  if (mode == TOUCH_SOUND) {
    if (!touchSoundLoop()) {
      mode = TRIVIA;
    }
  } else if (mode == TRIVIA) {
    if (!triviaLoop()) {
      mode = TOUCH_SOUND;
    }
  } else {
    DEBUG_PRINTLN("Unknown Mode!");
  }
}
