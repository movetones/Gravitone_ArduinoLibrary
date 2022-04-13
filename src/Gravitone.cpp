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
        handleButtons(i);
      }    
    }
  }

}

void Gravitone::handleButtons(int bid)
{
  static bool modeSwitch = false;
  
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
        if( activeMode == 0 ){
          setActiveMode( numModes - 1 );
        } else {
          setActiveMode( activeMode - 1 );
        }
      }
    } else {
      mode->button2(hardware.getButtonState(bid));
    }
    break;
  case GB3:
    if( modeSwitch ){
      if( hardware.getButtonState(bid) == BUTTON_PRESSED ){
        if( activeMode == (numModes-1) ){
          setActiveMode( 0 );
        } else {
          setActiveMode( activeMode + 1 );
        }
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

