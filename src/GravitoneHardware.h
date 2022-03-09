#ifndef GRAVITONE_HARDWARE_H
#define GRAVITONE_HARDWARE_H

#include "ICM_20948.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP23017.h>
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
                        handleEsp(true) {
          
  };
  
  ~GravitoneHardware() {};

  bool begin();
  
  void update(bool &newButtons, bool &newImu, bool &newDisplay);
  
  void enableAmp()   { setAmpState(true);  };
  void disableAmp()  { setAmpState(false); };
  bool getAmpState() { return ampState;    }
  void setAmpState(bool state);

  bool updateOrientation();
  float getYaw() { return yaw; }
  float getPitch() { return pitch; }
  float getRoll() { return roll; }

  bool buttonsAvailable();
  int lastButtonPress();
  int getButtonState(int id) { return buttonStates[id]; }
  
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
