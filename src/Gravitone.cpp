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


/** @file Gravitone.cpp
 *
 */

#include "Gravitone.h"

/** @brief call to initialize the gravitone
 */
bool Gravitone::begin()
{
  return hardware.begin();
}


/**
 * @brief Gravitone::setActiveMode
 * @param id
 */
void Gravitone::setActiveMode(int id) {
  if( activeMode >= 0 ){ // if there is a mode started
    Serial.print("stopping mode ");
    Serial.println(activeMode);
    mode->stop();
  }

  if( numModes > 0  && id >= 0 && id < numModes ){
    Serial.print("Setting activeMode to "); Serial.println(id);
    activeMode = id;
    mode = modes[id];
    if( init[activeMode] == false ){
      mode->begin();
      init[activeMode] = true;
    }
    mode->start();
  }
}

/**
 * @brief Gravitone::addMode
 * @param m
 */
void Gravitone::addMode(GravitoneMode *m) {
  modes[numModes] = m;
  modes[numModes]->setHardware(&hardware);
  init[numModes] = false;
  numModes += 1;
}

/**
 * @brief Gravitone::eventLoop
 */
void Gravitone::eventLoop()
{
  bool newButtons = false, newImu = false, newDisplay = false;
  uint8_t i;
  
  hardware.update(newButtons, newImu, newDisplay);
  
  if( newImu ){
    mode->onUpdateOrientation();
  }

  if( newDisplay ){
    mode->onUpdateDisplay();
  }
  
  if( newButtons ){
    for( i =0; i<12; i++ ){
      if( hardware.buttonsAvailable(i) ){
        handleButtons(i, hardware.getButtonState(i));
      }    
    }
  }

}

/**
 * @brief Gravitone::handleButtons
 * @param bid
 */
void Gravitone::handleButtons(int bid, butevent_t event)
{
  static bool modeSwitch = false;
  
  switch ( bid ){
  case GB1:
    if( event == BUTTON_PRESSED ){
      modeSwitch = true;
    } else if( event == BUTTON_RELEASED ){
      modeSwitch = false;
    }
    break;
  case GB2:
    if( modeSwitch ){
      if( event == BUTTON_PRESSED ){
        if( activeMode == 0 ){
          setActiveMode( numModes - 1 );
        } else {
          setActiveMode( activeMode - 1 );
        }
      }
    } else {
      mode->button2(event);
    }
    break;
  case GB3:
    if( modeSwitch ){
      if( event == BUTTON_PRESSED ){
        if( activeMode == (numModes-1) ){
          setActiveMode( 0 );
        } else {
          setActiveMode( activeMode + 1 );
        }
      }
    } else {
      mode->button3(event);
    }
    break;
  case GB4:
    mode->button4(event);
    break;
  case GB5:
    mode->button5(event);
    break;
  case GB6:
    mode->button6(event);
    break;
  case GB7:
    mode->button7(event);
    break;
  case GB8:
    mode->button8(event);
    break;
  case GB9:
    mode->button9(event);
    break;
  case GB10:
    mode->button10(event);
    break;
  case GB11:
    mode->button11(event);
    break;
  case GB12:
    mode->button12(event);
    break; 
  }
}

