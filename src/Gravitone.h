// Gravitone.h Gravitone library header
// Matt Ruffner Sept 20 2019

#ifndef GRAVITONE_H
#define GRAVITONE_H

#include <Audio.h>
#include <Arduino.h>

#include "config.h"
#include "GravitoneHardware.h"
#include "GravitoneMode.h"


class Gravitone {
public:
  Gravitone() : numModes(0), activeMode(-1) {};
  ~Gravitone() {};
  
  bool begin();
  
  void eventLoop();

  void setActiveMode(int id);
  int getActiveMode() { return activeMode; }
  
  void addMode(GravitoneMode *m);
  
  int numModes, activeMode;
  
  GravitoneMode *mode;
  
  GravitoneMode *modes[MAX_MODES];
  
  GravitoneHardware hardware;
};


#endif
