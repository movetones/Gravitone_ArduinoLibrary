/*
GravitoneHardware.h

Matt Ruffner 2022
MoveTones, LLC



*/

#ifndef GRAVITONE_HARDWARE_H
#define GRAVITONE_HARDWARE_H

#include "ICM_20948.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP23017.h>
#include <Geometry.h>
#include <Audio.h>

#include "config.h"
#include "util/battery.h"
#include "util/pins.h"
#include "util/buttons.h"
#include "scale.h"

#define MAX_PATCHES 20
#define MAX_MODES   10

class GravitoneHardware {
public:
  GravitoneHardware() : 
                        ampState(false),
                        lastDisplayUpdate(0), 
                        lastBatteryUpdate(0), 
                        lastButtonUpdate(0),
                        lastImuUpdate(0),
                        inactivityTimer(0),
                        handleEsp(true),
                        ax(0),ay(0),az(0),
                        gx(0),gy(0),gz(0),
                        mx(0),my(0),mz(0) {
    //pointer(0) = 0.0;
    pointer(0) = 0.0;
    pointer(1) = 0.0;
    pointer(2) = 0.0;
    
    origin(0) = 0.0;
    origin(1) = -1.0;
    origin(2) = 0.0;
          
  };
  
  ~GravitoneHardware() {};

  bool begin();
  
  void update(bool &newButtons, bool &newImu, bool &newDisplay);
  
  void enableAmp()   { setAmpState(true);  };
  void disableAmp()  { setAmpState(false); };
  bool getAmpState() { return ampState;    }
  void setAmpState(bool state);

  bool updateOrientation();
  double getYaw() { return yaw; }
  double getPitch() { return pitch; }
  double getRoll() { return roll; }
  double getQ0() { return q0; }
  double getQ1() { return q1; }
  double getQ2() { return q2; }
  double getQ3() { return q3; }
  double getAx() { return ax; }
  double getAy() { return ax; }
  double getAz() { return ax; }

  bool buttonsAvailable();
  bool buttonsAvailable(int i);
  int lastButtonPress();
  int getButtonState(int id) { return buttonStates[id]; }
  uint8_t* getButtonStates() { return buttonStates; }
  
  void sleep();
  void wake();
  
  AudioOutputI2S           i2s1;           //xy=793,403

  static ICM_20948_I2C myICM;
  static Adafruit_MCP23017 mcp;
  static Adafruit_SSD1306 display;
  
private:
  void updateBattery();
  void updateButtons();
  bool initImu();
  void initScreen();
  void initButtons();
  void imuSleep();
  void imuWake();
  
  BLA::Matrix<3> pointer, origin;
  bool ampState;
  bool handleEsp;
  double yaw, pitch, roll;
  double q0, q1, q2, q3;
  double ax,ay,az,gx,gy,gz,mx,my,mz;
  unsigned long lastImuUpdate, lastDisplayUpdate, lastBatteryUpdate, lastButtonUpdate;
  unsigned long inactivityTimer;
  uint16_t buttonUpdateInterval, displayUpdateInterval, batteryUpdateInterval;
  uint8_t buttonStates[12]; // states in buttons.h
  bool buttonStatesChanged[12];
};

#endif
