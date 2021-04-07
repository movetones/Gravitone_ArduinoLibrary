// Gravitone.h Gravitone library header
// Matt Ruffner Sept 20 2019

#ifndef GRAVITONE_H
#define GRAVITONE_H

#include <Arduino.h>

#include "scale.h"
#include "util/pins.h"
#include "util/buttons.h"

#include <ICM_20948.h>
#include <MadgwickAHRS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP23017.h>

//#define GTONE_USE_DMP 1

class Gravitone {

public:
  Gravitone() : lastImuRead(0){};
  ~Gravitone() {};
  
  bool begin();
  
  void setSpeakerState(bool state);
  bool getSpeakerState() { return speakerState; }

  bool updateOrientation();
  float getYaw() { return yaw; }
  float getPitch() { return pitch; }
  float getRoll() { return roll; }

  void updateBattery();
  void updateVolume(float vol);

  void updateButtons();
  bool buttonsAvailable();
  int lastButtonPress();
  bool getButtonState(int id) { return buttonStates[id]; }

  static ICM_20948_I2C myICM;
  static Adafruit_MCP23017 mcp;
  static Adafruit_SSD1306 display;
  
private:
  Madgwick filter;

  void imuSleep();
  void imuWake();

  bool initImu();
  void initScreen();
  void initButtons();

  float yaw, pitch, roll;
  float ax,ay,az,gx,gy,gz,mx,my,mz;
  unsigned long lastImuRead;

  bool buttonStates[12];
  bool buttonStatesChanged[12];
  bool speakerState;
};


#endif
