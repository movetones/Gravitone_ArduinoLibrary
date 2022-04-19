/* Gravitone Library
 * Copyright (c) 2022, Matt Ruffner <matthew.ruffner@movetones.com>
 * MoveTones, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

#define MAX_PATCHES 20 //! maximum number of Audio
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
                        heading(0),
                        pitch(0),
                        roll(0),
                        ax(0),ay(0),az(0),
                        gx(0),gy(0),gz(0),
                        mx(0),my(0),mz(0) {

    xOrigin(0) = 1.0;
    xOrigin(1) = 0.0;
    xOrigin(2) = 0.0;
    
    yOrigin(0) = 0.0;
    yOrigin(1) = -1.0;
    yOrigin(2) = 0.0;
    
    zOrigin(0) = 0.0;
    zOrigin(1) = 0.0;
    zOrigin(2) = -1.0;
          
  };
  
  ~GravitoneHardware() {};

  /**
   * @brief begin
   * @return if initialization was successful or not
   */
  bool begin();
  
  /**
   * @brief update
   * @param newButtons set true if new buttons have been pressed
   * @param newImu set true if new IMU data is available
   * @param newDisplay set true if its time for a display update
   */
  void update(bool &newButtons, bool &newImu, bool &newDisplay);
  /**
   * @brief enableAmp
   *
   */
  void enableAmp()   { setAmpState(true);  };
  void disableAmp()  { setAmpState(false); };
  bool getAmpState() { return ampState;    }
  void setAmpState(bool state);

  bool updateOrientation();
  double getHeading() { return heading; }
  double getPitch() { return pitch; }
  double getRoll() { return roll; }
  double getQ0() { return q0; }
  double getQ1() { return q1; }
  double getQ2() { return q2; }
  double getQ3() { return q3; }
  double getAx() { return ax; }
  double getAy() { return ay; }
  double getAz() { return az; }

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
  
  BLA::Matrix<3> xPointer, yPointer, 
                 zPointer, xOrigin, 
                 yOrigin, zOrigin;
  bool ampState;
  bool handleEsp;
  double heading, pitch, roll;
  double q0, q1, q2, q3;
  double ax,ay,az,gx,gy,gz,mx,my,mz;
  unsigned long lastImuUpdate, lastDisplayUpdate, lastBatteryUpdate, lastButtonUpdate;
  unsigned long inactivityTimer;
  uint16_t buttonUpdateInterval, displayUpdateInterval, batteryUpdateInterval;
  uint8_t buttonStates[12]; // states in buttons.h
  bool buttonStatesChanged[12];
};

#endif
