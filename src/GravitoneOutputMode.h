#ifndef GRAVITONE_OUTPUT_MODE_H
#define GRAVITONE_OUTPUT_MODE_H

#include "GravitoneMode.h"

class GravitoneOutputMode : public GravitoneMode {
public:
  GravitoneOutputMode() : volume(1) {};
  ~GravitoneOutputMode() {};
  
  void start() {
    addPatch( new AudioConnection(amp1, fade1) );
    addPatch( new AudioConnection(fade1, 0, i2s1, 0) );
    addPatch( new AudioConnection(fade1, 0, i2s1, 1) );
    amp1.gain(0.2);
    fade1.fadeOut(500);
    hardware->enableAmp();
    drawVolume();
    setVolume();
    
    Serial.print("End of GravitoneOutputMode start(), numPatches = ");
    Serial.println(numPatches);
  }
  
  void stop() {
    clearPatches();
    hardware->display.clearDisplay();
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
  AudioEffectFade          fade1;          //xy=492,402
  AudioOutputI2S           i2s1;           //xy=793,403 
  
  int volume;                  
};


#endif