// Gravitone.h Gravitone library header
// Matt Ruffner Sept 20 2019

#ifndef GRAVITONE_H
#define GRAVITONE_H

#include <Audio.h>
#include <Arduino.h>

#include "scale.h"
#include "util/pins.h"
#include "buttons.h"

// use local copy of madgwick that has the correct initial orientation specified
// this helps the filter stabilize quicker when the IMU is initially oriented the way
// it is in the gravitone
#include "MadgwickAHRS.h"

#include "ICM_20948.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP23017.h>

#define QUAT_ANIMATION
#define GTONE_USE_DMP 
#define SERIAL_PORT Serial

#define MAX_PATCHES 75

typedef const Adafruit_SSD1306  gtdisplay_t;
typedef const Adafruit_MCP23017 gtbuttons_t;

class GravitoneMode {
public:
  GravitoneMode() : numPatches(0) {
    addPatch( new AudioConnection(amp1, fade1) );
    addPatch( new AudioConnection(fade1, 0, i2s1, 0) );
    addPatch( new AudioConnection(fade1, 0, i2s1, 1) );
    amp1.gain(0.2);
    fade1.fadeOut(500);
  };
  
  ~GravitoneMode();
  
  void addPatch(AudioConnection *cable);
  
  virtual void start(gtdisplay_t &display) = 0;
  
  // gravitoneMode base class handles the three menu buttons
  // used for volume control and switching modes
  //virtual void button1(butevent_t event, gtdisplay_t & display);
  //virtual void button2(butevent_t event, gtdisplay_t & display);
  //virtual void button3(butevent_t event, gtdisplay_t & display);
  
  
  virtual void button4(butevent_t event, gtdisplay_t & display) = 0;
  virtual void button5(butevent_t event, gtdisplay_t & display) = 0;
  virtual void button6(butevent_t event, gtdisplay_t & display) = 0;
  virtual void button7(butevent_t event, gtdisplay_t & display) = 0;
  virtual void button8(butevent_t event, gtdisplay_t & display) = 0;
  virtual void button9(butevent_t event, gtdisplay_t & display) = 0;
  virtual void button10(butevent_t event, gtdisplay_t & display) = 0;
  virtual void button11(butevent_t event, gtdisplay_t & display) = 0;
  virtual void button12(butevent_t event, gtdisplay_t & display) = 0;
  
  virtual void onUpdateOrientation(double y, double p, double r ) {};
  
  AudioConnection *patchCoords[3];
  int numPatches;
  
  AudioAmplifier           amp1;           //xy=323,403
  AudioEffectFade          fade1;          //xy=492,402
  AudioOutputI2S           i2s1;           //xy=793,403 
};



class Gravitone {

public:
  Gravitone() : lastDisplayUpdate(0), 
                lastBatteryUpdate(0), 
                lastButtonUpdate(0),
                lastImuUpdate(0),
                inactivityTimer(0),
                volume(1) {};
  ~Gravitone() {};
  
  bool begin();
  
  void eventLoop();
  
  void setSpeakerState(bool state);
  bool getSpeakerState() { return speakerState; }

  bool updateOrientation();
  float getYaw() { return yaw; }
  float getPitch() { return pitch; }
  float getRoll() { return roll; }

  void updateBattery();
  void drawVolume();
  void cycleVolume(int dir);
  void setVolume(int lvl); // 0 through 4 
  void setVolume(); // uses member var
  
  void setMode(GravitoneMode *m);
  GravitoneMode * getMode();

  void updateButtons();
  bool buttonsAvailable();
  int lastButtonPress();
  int getButtonState(int id) { return buttonStates[id]; }

  static ICM_20948_I2C myICM;
  static Adafruit_MCP23017 mcp;
  static Adafruit_SSD1306 display;
  
  GravitoneMode *mode;
  
private:
  //Madgwick filter;

  void imuSleep();
  void imuWake();

  bool initImu();
  void initScreen();
  void initButtons();

  int volume;
  double yaw, pitch, roll;
  double q0, q1, q2, q3;
  double ax,ay,az,gx,gy,gz,mx,my,mz;
  unsigned long lastImuUpdate, lastDisplayUpdate, lastBatteryUpdate, lastButtonUpdate;
  unsigned long inactivityTimer;
  uint16_t buttonUpdateInterval, displayUpdateInterval, batteryUpdateInterval, imuUpdateInterval;
  uint8_t buttonStates[12]; // states in buttons.h
  bool buttonStatesChanged[12];
  bool speakerState;
};


#endif
