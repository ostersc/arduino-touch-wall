bool touchSoundLoop() {
  if (MPR121.touchStatusChanged()) {
    MPR121.updateTouchData();
    if (MPR121.getNumTouches() <= 1) {
      for (byte i = 0; i < 12; i++) { // Check which electrodes were pressed
        if (MPR121.isNewTouch(i)) {
          DEBUG_PRINT("pin ");
          DEBUG_PRINT(i);
          DEBUG_PRINTLN(" was just touched");
          digitalWrite(LED_BUILTIN, HIGH);
          if (MP3player.isPlaying()) {
            MP3player.stopTrack();
          }

          String f=characters[i].getRandomAudioFile();
          char chars[f.length() + 1];
          f.toCharArray(chars, f.length() + 1);

          //randomly select from character audio files
          MP3player.playMP3(chars);
          DEBUG_PRINT("playing track ");
          DEBUG_PRINTLN(i);
        } else if (MPR121.isNewRelease(i)) {
          DEBUG_PRINT("pin ");
          DEBUG_PRINT(i);
          DEBUG_PRINTLN(" is no longer being touched");
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
    }else if(MPR121.getNumTouches() == 2){
      //player wants to switch modes
      return false;
    }
  }
  return true;
}
