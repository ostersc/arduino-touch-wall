#ifndef GAME_H
#define GAME_H

#include "Arduino.h"
#include "CharacterConfig.h"
#include "MP3Util.h"

const int MAX_ROUND_DURATION = 12000;

class Game {
  public:
    enum GameState {
      waiting_to_start,
      waiting_for_round,
      playing,
      waiting_to_end,
      ended
    };
    //construtors
    Game();
    Game(byte rounds);

    byte getScore();
    long getRoundDuration();
    bool answer(byte i);
    void noAnswer();

    //state operations
    GameState getState();
    bool nextRound(CharacterConfig c); // false if no remaining rounds
    void start();
    void end();
    void reset();

    //deconstructor
    ~Game();
  private:
    byte currentScore, currentRound, numRounds;
    GameState gameState;

    //round stuff
    String roundAudioFile;
    byte roundExpectedSensorPin;
    long roundStartTime;
};

#endif
