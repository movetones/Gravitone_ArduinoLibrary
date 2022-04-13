#ifndef GRAVITONE_OUTPUT_MODE_H
#define GRAVITONE_OUTPUT_MODE_H

#include "GravitoneMode.h"

class GravitoneOutputMode : public GravitoneMode {
public:
  GravitoneOutputMode() : volume(1) {};
  ~GravitoneOutputMode() {  };
  
  void begin() {
    addPatch( new AudioConnection(amp1, 0, hardware->i2s1, 0) );
    addPatch( new AudioConnection(amp1, 0, hardware->i2s1, 1) );
  }

  void start() {
    hardware->enableAmp();
    drawVolume();
    setVolume();
    reconnectPatches();
    Serial.print("End of GravitoneOutputMode start(), numPatches = ");
    Serial.println(numPatches);
  }
  
  void stop() {
    disconnectPatches();
    hardware->disableAmp();
  }
  
  virtual void button2(butevent_t event) {  
    if( event == BUTTON_PRESSED ){
      cycleVolume(VOL_DOWN);
    }
  };
  
  virtual void button3(butevent_t event) {
    if( event == BUTTON_PRESSED ){
      cycleVolume(VOL_UP);
    }
  }
  
  void drawVolume();
  void cycleVolume(int dir);
  void setVolume(int lvl); // 0 through 4 
  void setVolume(); // uses member var

  AudioAmplifier           amp1;           //xy=323,403
  
  int volume;                  
};


#endif
