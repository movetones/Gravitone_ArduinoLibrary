#ifndef GRAVITONE_MODE_H
#define GRAVITONE_MODE_H

#include <Arduino.h>
#include <Audio.h>
#include "GravitoneHardware.h"
#include "util/buttons.h"
#include "util/battery.h"
#include "util/volume.h"
#include "config.h"

class GravitoneMode {
public:
  GravitoneMode() : numPatches(0) {};
  
  ~GravitoneMode() { };
  
  void addPatch(AudioConnection *cable);
  void reconnectPatches();
  void disconnectPatches();
  
  void setHardware(GravitoneHardware *h) { hardware = h; };
  
  virtual void begin() {};
  virtual void start() {};
  virtual void stop() {};
  
  virtual void button1(butevent_t event) {};
  virtual void button2(butevent_t event) {};
  virtual void button3(butevent_t event) {};
  virtual void button4(butevent_t event) {};
  virtual void button5(butevent_t event) {};
  virtual void button6(butevent_t event) {};
  virtual void button7(butevent_t event) {};
  virtual void button8(butevent_t event) {};
  virtual void button9(butevent_t event) {};
  virtual void button10(butevent_t event) {};
  virtual void button11(butevent_t event) {};
  virtual void button12(butevent_t event) {};
  
  virtual void onUpdateOrientation() {};
  virtual void onUpdateDisplay() {};
  
  virtual const char *getName() {};
  
  GravitoneHardware *hardware;
  
  AudioConnection *patchCoords[MAX_PATCHES];
  int numPatches;
};


#endif
