#include "WaveMode.h"

WaveMode::WaveMode() {
  
  activeWaveform = WAVEFORM_SINE;
  playing = false;
  continuous = false;
  octaveShift = 0;
  note = 440;
  int note = 5;

  scale = new gs_Scale(GS_SCALE_PENT_MAJOR_PATTERN, gs_Notes[NOTE_G.pos], 1);
  
  addPatch( new AudioConnection(waveform1, amp1) );
  
  waveform1.begin(0.8, 220, activeWaveform);
};

WaveMode::~WaveMode() {}

void WaveMode::start(gtdisplay_t & display) {
  display.setTextSize(0);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.print("WaveMode");
  display.fillRect(68,12,80,10,BLACK);
  display.setCursor(68,12);
  if( continuous ){
    display.print("Continuous");
  } else {
    display.print("Scale");
  }
  display.fillRect(0,12, 50, 10, BLACK);
  display.setCursor(0,12);
  display.print("Sine");
  display.setCursor(0, 0);
  display.fillRect(0,0,90,10,BLACK);
  display.print(scale->getName());
};

void WaveMode::onUpdateOrientation(double yaw, double pitch, double roll) {

  note = map(pitch, -70, 70, 0, scale->getNoteCount());
  freq = scale->getNote(note)->freq * pow(2, octaveShift);

  //Serial.print("on update orientation ");
  //Serial.print(millis()); Serial.print(" "); Serial.print(yaw); Serial.print(" "); Serial.print(pitch); Serial.print(" "); Serial.println(roll);

  // continuous tone
  if( continuous )
    freq = scale->unison.freq*pow(2,(float)(map(pitch, -60, 60, 0, 12))/12.0)*pow(2, octaveShift);
  //else 
   waveform1.frequency(freq);
}

void WaveMode::button4(butevent_t event, gtdisplay_t & display){
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 4 pressed");
    playing = true;
    fade1.fadeIn(20);
  } else if( event == BUTTON_RELEASED ){
    playing = false;
    fade1.fadeOut(100);
  }  
}

void WaveMode::button5(butevent_t event, gtdisplay_t & display) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 5 pressed");
  }
}

void WaveMode::button6(butevent_t event, gtdisplay_t & display) {
  if( event == BUTTON_PRESSED ){
    Serial.println("button 6 pressed");
    scaleIndex = (scaleIndex + 1) % 12;
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleTypeIndex], gs_Notes[scaleIndex], 1);
    display.setCursor(0, 0);
    display.fillRect(0,0,90,10,BLACK);
    display.print(scale->getName());
  }
}

void WaveMode::button7(butevent_t event, gtdisplay_t & display) {
  if( event == BUTTON_PRESSED ){
    Serial.println("button 7 pressed");
    continuous = !continuous;
    display.fillRect(68,12,80,10,BLACK);
    display.setCursor(68,12);
    if( continuous ){
      display.print("Continuous");
    } else {
      display.print("Scale");
    }
  }
}

void WaveMode::button8(butevent_t event, gtdisplay_t & display) {
  if( event == BUTTON_PRESSED ){
    Serial.println("button 8 pressed");
    scaleTypeIndex = (scaleTypeIndex + 1) % GS_NUM_SCALE_PATTERNS;
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleTypeIndex], gs_Notes[scaleIndex], 1);
    display.setCursor(0, 0);
    display.fillRect(0,0,90,10,BLACK);
    display.print(scale->getName());
  }
}

void WaveMode::button9(butevent_t event, gtdisplay_t & display) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 9 pressed");
    display.fillRect(0,12, 50, 10, BLACK);
    display.setCursor(0,12);
    activeWaveform = WAVEFORM_TRIANGLE;
    waveform1.begin(activeWaveform);
    display.print("Triangle");
  }
}

void WaveMode::button10(butevent_t event, gtdisplay_t & display) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 10 pressed");
    display.fillRect(0,12, 50, 10, BLACK);
    display.setCursor(0,12);
    activeWaveform = WAVEFORM_SINE;
    waveform1.begin(activeWaveform);
    display.print("Sine");
  }
}

void WaveMode::button11(butevent_t event, gtdisplay_t & display) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 11 pressed");
    display.fillRect(0,12, 50, 10, BLACK);
    display.setCursor(0,12);
    activeWaveform = WAVEFORM_SAWTOOTH;
    waveform1.begin(activeWaveform);
    display.print("Sawtooth");
  }
}

void WaveMode::button12(butevent_t event, gtdisplay_t & display) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 12 pressed");
    activeWaveform = WAVEFORM_SQUARE;
    waveform1.begin(activeWaveform);
    display.fillRect(0,12, 50, 10, BLACK);
    display.setCursor(0,12);
    display.print("Square");
  }
}
