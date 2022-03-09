// Gravitone.h Gravitone library header
// Matt Ruffner Sept 20 2019

#ifndef GRAVITONE_H
#define GRAVITONE_H

#include <Audio.h>
#include <Arduino.h>

#include "scale.h"
#include "util/pins.h"
#include "util/buttons.h"

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

#define GTONE_WIFI

#define MAX_PATCHES 75
#define MAX_MODES   10


#define VOL_UP 1
#define VOL_DOWN -1

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
  
  void update(bool &newButtons, bool &newImu);
  
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

class GravitoneMode {
public:
  GravitoneMode() : numPatches(0) {};
  
  ~GravitoneMode() { 
    clearPatches();
  };
  
  void addPatch(AudioConnection *cable);
  void clearPatches();
  
  void setHardware(GravitoneHardware *h) { hardware = h; };
  
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
  
  virtual const char *getName() {};
  
  GravitoneHardware *hardware;
  
  AudioConnection *patchCoords[MAX_PATCHES];
  int numPatches;
};

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
  
  static GravitoneHardware hardware;
};


#endif
