#include "Gravitone.h"
//#include "util/chords.h"
//#include "util/volume.h"
//#include "util/battery.h"


bool Gravitone::begin()
{
  return hardware.begin();
}


/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::setActiveMode(int id) {
  if( activeMode >= 0 ){ // if there is a mode started
    Serial.print("stopping mode ");
    Serial.println(activeMode);
    mode->stop();
  }

  if( numModes > 0  && id >= 0 && id < numModes ){\
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

/********************************************************************************************
 *
 *
 *******************************************************************************************/
void Gravitone::addMode(GravitoneMode *m) {
  modes[numModes] = m;
  modes[numModes]->setHardware(&hardware);
  init[numModes] = false;
  numModes += 1;
}


void Gravitone::eventLoop()
{
  static bool modeSwitch = false;
  bool newButtons = false, newImu = false, newDisplay = false;
  
  hardware.update(newButtons, newImu, newDisplay);
  
  if( newImu ){
    mode->onUpdateOrientation();
  }

  if( newDisplay ){
    mode->onUpdateDisplay();
  }
  
  if( newButtons ){
    int bid = hardware.lastButtonPress();
    switch ( bid ){
      case GB1:
        if( hardware.getButtonState(bid) == BUTTON_PRESSED ){
          modeSwitch = true;
        } else if( hardware.getButtonState(bid) == BUTTON_RELEASED ){
          modeSwitch = false;
        }
        break;
      case GB2:
        if( modeSwitch ){
          if( hardware.getButtonState(bid) == BUTTON_PRESSED ){
            if( activeMode >= numModes ) activeMode = 0;
            else activeMode ++;           
            setActiveMode( activeMode );
          }
        } else {
          mode->button2(hardware.getButtonState(bid));
        }
        break;
      case GB3:
        if( modeSwitch ){
          if( hardware.getButtonState(bid) == BUTTON_PRESSED ){
            if( activeMode == 0 ) activeMode = numModes - 1;
            else activeMode --;
            setActiveMode( activeMode );
          }
        } else {
          mode->button3(hardware.getButtonState(bid));
        }
        break;
      case GB4:
        mode->button4(hardware.getButtonState(bid));
        break;
      case GB5:
        mode->button5(hardware.getButtonState(bid));
        break;
      case GB6:
        mode->button6(hardware.getButtonState(bid));
        break;
      case GB7:
        mode->button7(hardware.getButtonState(bid));
        break;
      case GB8:
        mode->button8(hardware.getButtonState(bid));
        break;
      case GB9:
        mode->button9(hardware.getButtonState(bid));
        break;
      case GB10:
        mode->button10(hardware.getButtonState(bid));
        break;
      case GB11:
        mode->button11(hardware.getButtonState(bid));
        break;
      case GB12:
        mode->button12(hardware.getButtonState(bid));
        break; 
    }
  }

}

