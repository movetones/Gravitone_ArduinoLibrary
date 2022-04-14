#include "WaveMode.h"

WaveMode::WaveMode() {
  activeWaveform = WAVEFORM_SINE;
  octaveShift = 0;
  baseNote = 440;
  fade1.fadeOut(200);
  fade2.fadeOut(200);
  fade3.fadeOut(200);
  waveform1.begin(0.9, 440, activeWaveform);
  waveform2.begin(0.9, 220, activeWaveform);
  waveform3.begin(0.9, 880, activeWaveform);
  addPatch( new AudioConnection(waveform1, fade1) );
  addPatch( new AudioConnection(waveform2, fade2) );
  addPatch( new AudioConnection(waveform3, fade3) );
  addPatch( new AudioConnection(fade1, 0, mixer1, 0) );
  addPatch( new AudioConnection(fade2, 0, mixer1, 1) );
  addPatch( new AudioConnection(fade3, 0, mixer1, 2) );
  addPatch( new AudioConnection(mixer1, 0, amp1, 0) );
  mixer1.gain(0, 0.9);
  mixer1.gain(1, 0.9);
  mixer1.gain(2, 0.9);
};

WaveMode::~WaveMode() {
}

const char *WaveMode::getName()
{
  return "WaveMode";
}

void WaveMode::start() 
{
  GravitoneOutputMode::start();

  waveform1.amplitude(0.9);
  waveform2.amplitude(0.9);
  waveform3.amplitude(0.9);
};

void WaveMode::stop()
{
  //waveform1.amplitude(0);
  //delete scale;
  GravitoneOutputMode::stop();
  //clearPatches();
  Serial.println("stoppping WaveMode");
  hardware->display.clearDisplay();
}

void WaveMode::onUpdateDisplay()
{
  hardware->display.setTextSize(0);
  hardware->display.setTextColor(WHITE, BLACK);
  hardware->display.setCursor(0, 0);
  hardware->display.print("WaveMode");


  hardware->display.fillRect(0,12, 50, 10, BLACK);
  hardware->display.setCursor(0,12);
  switch( activeWaveform ) {
  case WAVEFORM_SINE:
    hardware->display.print("Sine");
    break;
  case WAVEFORM_SQUARE:
    hardware->display.print("Square");
    break;
  case WAVEFORM_TRIANGLE:
    hardware->display.print("Triangle");
    break;
  case WAVEFORM_SAWTOOTH:
    hardware->display.print("Sawtooth");
    break;
  }
}

void WaveMode::onUpdateOrientation() 
{
  double heading = hardware->getHeading();
  double pitch = hardware->getPitch();
  double roll  = hardware->getRoll();

  Serial.print(millis()); Serial.print(" "); Serial.print(heading); Serial.print(" "); Serial.print(pitch); Serial.print(" "); Serial.println(roll);

  // continuous tone
  freq  = baseNote*pow(2,(float)(map(pitch, 165, 15, 0, 12))/12.0)*pow(2, octaveShift);
  freq2 = baseNote*pow(2,(float)(map(roll, 15, 165, 0, 12))/12.0)*pow(2, octaveShift);
  freq3 = baseNote*pow(2,(float)(map(heading, -180, 180, 0, 12))/12.0)*pow(2, octaveShift);
  
  waveform1.frequency(freq);
  waveform2.frequency(freq2);
  waveform3.frequency(freq3);
}

void WaveMode::button4(butevent_t event){
  if( event == BUTTON_PRESSED ){
    fade1.fadeIn(20);
  } else if( event == BUTTON_RELEASED ){
    fade1.fadeOut(100);
  }  
}

void WaveMode::button5(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    fade2.fadeIn(20);
  } else if( event == BUTTON_RELEASED ){
    fade2.fadeOut(100);
  }
}

void WaveMode::button6(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    fade3.fadeIn(20);
  } else if( event == BUTTON_RELEASED) {
    fade3.fadeOut(100);
  }
}

void WaveMode::button7(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    fade1.fadeIn(20);
    fade3.fadeIn(20);
  } else if( event == BUTTON_RELEASED) {
    fade1.fadeOut(100);
    fade3.fadeOut(100);
  }
}

void WaveMode::button8(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    fade2.fadeIn(20);
    fade3.fadeIn(20);
  } else if( event == BUTTON_RELEASED) {
    fade2.fadeOut(100);
    fade3.fadeOut(100);
  }
}

void WaveMode::button9(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    fade1.fadeIn(20);
    fade2.fadeIn(20);
  } else if( event == BUTTON_RELEASED) {
    fade1.fadeOut(100);
    fade2.fadeOut(100);
  }
}

void WaveMode::button10(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    if ( activeWaveform == WAVEFORM_SAWTOOTH) {
      activeWaveform = WAVEFORM_SINE;
      waveform1.begin(activeWaveform);
      waveform2.begin(activeWaveform);
    } else if( activeWaveform == WAVEFORM_SINE ){
      activeWaveform = WAVEFORM_SAWTOOTH;
      waveform1.begin(activeWaveform);
      waveform2.begin(activeWaveform);
    }
  }
}

void WaveMode::button11(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    baseNote = map(hardware->getPitch(), 165, 15, 220, 880);
  }
}

void WaveMode::button12(butevent_t event) {
    if( event == BUTTON_PRESSED ){
    fade1.fadeIn(20);
    fade2.fadeIn(20);
    fade3.fadeIn(20);
  } else if( event == BUTTON_RELEASED) {
    fade1.fadeOut(100);
    fade2.fadeOut(100);
    fade3.fadeOut(100);
  }
}
