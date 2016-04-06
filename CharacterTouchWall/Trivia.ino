bool triviaLoop() {
  //DEBUG_PRINT("Trivia loop in game state: ");
  //DEBUG_PRINTLN(game.getState());

  //If curr round is null/negative and touch input :start game/round
  if (game.getState() == Game::waiting_to_start) {
    //DEBUG_PRINT("Waiting to start game");
    if (MPR121.touchStatusChanged()) {
      MPR121.updateTouchData();
      if (MPR121.getNumTouches() == 1) {
        DEBUG_PRINT("Starting game.");
        game.start();
      }
    }
  } else if (game.getState() == Game::waiting_for_round) { //need to load next round
    //Pick a random character from game config
    DEBUG_PRINT("Starting next round.");
    game.nextRound(characters[random(12)]);
  } else if (game.getState() == Game::playing) { //main game loop
    if (game.getRoundDuration() > MAX_ROUND_DURATION) {
      DEBUG_PRINT("No answer.");
      game.noAnswer();
    } else if (MPR121.touchStatusChanged()) {
      MPR121.updateTouchData();
      if (MPR121.getNumTouches() == 1) {
        // Check which electrodes were pressed
        for (byte i = 0; i < 12; i++) {
          if (MPR121.isNewTouch(i)) {
            //check the answer
            DEBUG_PRINT("Answering ");
            DEBUG_PRINTLN(i);
            game.answer(i);
            break;
          }
        }
      } else if (MPR121.getNumTouches() == 2) {
        //reset game state and switch modes
        game.reset();
        return false;
      }
    }
  } else if (game.getState() == Game::waiting_to_end) { //game is over
    DEBUG_PRINT("Ending game.");
    game.end();
  } else if (game.getState() == Game::ended) {  //go back to waiting for a new game to start
    DEBUG_PRINT("Resetting and switching modes.");
    //reset game state and switch modes
    game.reset();
    return false;
  } else {
    DEBUG_PRINT("Game in unknown state!");
    DEBUG_PRINT(game.getState());
  }

  return true;
}
