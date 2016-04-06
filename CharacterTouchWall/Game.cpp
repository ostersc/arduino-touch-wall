//comment out to disable debug output
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print (x)
#define DEBUG_PRINTLN(x)  Serial.println (x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#include "Game.h"

#define CORRECT  "FX/correct.mp3"
#define INCORRECT  "FX/wrong.mp3"
#define ROUND  "FX/round.mp3"
#define TIMEOUT  "FX/timeout.mp3"
#define START  "FX/start.mp3"
//#define END  "FX/end.mp3"
#define SCORE  "FX/score.mp3"

//apparently you need to initialize the static member
//TODO: see if i still need this to be static once I removed the delegating constructor
//Game::GameState Game::gameState;

Game::Game() {
  reset();
}

Game::Game(byte rounds) {
  reset();
  numRounds = rounds;
}

bool Game::nextRound(CharacterConfig c) {
  if (++currentRound > numRounds) {
    gameState = Game::waiting_to_end;
    return false;
  } else {
    //Play sound (round.mp3 + (round number).mp3) to announce round, sleep
    MP3Util::playMP3Blocking(ROUND);
    MP3Util::sayNumber(currentRound);
    delay(250);

    DEBUG_PRINT("Starting round ");
    DEBUG_PRINTLN(currentRound);
    roundExpectedSensorPin = c.getSensorPin();
    roundAudioFile = c.getRandomAudioFile();

    // play the audio file for the round
    char chars[roundAudioFile.length() + 1];
    roundAudioFile.toCharArray(chars, roundAudioFile.length() + 1);

    DEBUG_PRINT("Playing track ");
    DEBUG_PRINTLN(chars);
    MP3Util::playMP3(chars);

    DEBUG_PRINT("Expecting answer ");
    DEBUG_PRINTLN(roundExpectedSensorPin);

    gameState = Game::playing;
    roundStartTime = millis();
    return true;
  }
}

boolean Game::answer(byte pin) {
  //if only correct touched:
  if (pin == roundExpectedSensorPin) {
    //Subtract current time - round start time, determine score
    byte score = 10 - (getRoundDuration() / 1000);
    if (score > 0) {
      currentScore += score;
    }
    DEBUG_PRINT("Correct.  Score was:");
    DEBUG_PRINTLN(score);

    MP3Util::playMP3Blocking(CORRECT);
    MP3Util::sayNumber(score);
  } else {
    DEBUG_PRINT("Incorrect.  Correct answer was:");
    DEBUG_PRINTLN(roundExpectedSensorPin);
    //Play incorrect.mp3; sleep 1 sec
    MP3Util::playMP3Blocking(INCORRECT);
    //delay(1000);
    //FUTURE: Play correction.mp3 +  correct spoken name
  }
  DEBUG_PRINT("Current score is:");
  DEBUG_PRINTLN(currentScore);
  gameState = Game::waiting_for_round;
}

void Game::noAnswer() {
  DEBUG_PRINT("No answer.  Current score is:");
  DEBUG_PRINTLN(currentScore);
  //Play timeout.mp3 ; sleep 1 sec
  MP3Util::playMP3Blocking(TIMEOUT);
  //FUTURE: Play correction.mp3 +  correct spoken name
  gameState = Game::waiting_for_round;
}


Game::GameState Game::getState() {
  return gameState;
}

byte Game::getScore() {
  return currentScore;
}

long Game::getRoundDuration() {
  return millis() - roundStartTime;
}

void Game::start() {
  DEBUG_PRINTLN("Starting.");
  //play start.mp3 to announce start of game
  MP3Util::playMP3Blocking(START);
  gameState = Game::waiting_for_round;
}

void Game::end() {
  DEBUG_PRINTLN("Ending.");
  //TODO: play end sound
  //MP3Util::playMP3Blocking(END);

  DEBUG_PRINT("Total score is:");
  DEBUG_PRINTLN(currentScore);

  //announce score (score.mp3 + spoken currentScore)
  MP3Util::playMP3Blocking(SCORE);
  MP3Util::sayNumber(currentScore);

  gameState = Game::ended;
}

void Game::reset() {
  DEBUG_PRINTLN("Resetting. Touch any sensor to start a new game.");
  gameState = Game::waiting_to_start;
  currentScore = currentRound = roundExpectedSensorPin = 0;
  roundStartTime = 0;
  roundAudioFile = "";
}

Game::~Game() {
}

