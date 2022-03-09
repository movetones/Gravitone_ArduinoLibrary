#include "WaveMode.h"

WaveMode::WaveMode() {
  
  activeWaveform = WAVEFORM_SINE;
  playing = false;
  continuous = false;
  octaveShift = 0;
  note = 440;
  int note = 5;
};

WaveMode::~WaveMode() {}

const char *WaveMode::getName()
{
  return "WaveMode";
}

void WaveMode::start() 
{
  GravitoneOutputMode::start();

  scale = new gs_Scale(GS_SCALE_PENT_MAJOR_PATTERN, gs_Notes[NOTE_G.pos], 1);
  
  Serial.println("created scale");
  
  addPatch( new AudioConnection(waveform1, amp1) );
  
  waveform1.begin(0.8, 220, activeWaveform);

  hardware->display.setTextSize(0);
  hardware->display.setTextColor(WHITE, BLACK);
  hardware->display.setCursor(0, 0);
  hardware->display.print("WaveMode");
  hardware->display.fillRect(68,12,80,10,BLACK);
  hardware->display.setCursor(68,12);
  if( continuous ){
    hardware->display.print("Continuous");
  } else {
    hardware->display.print("Scale");
  }
  hardware->display.fillRect(0,12, 50, 10, BLACK);
  hardware->display.setCursor(0,12);
  hardware->display.print("Sine");
  hardware->display.setCursor(0, 0);
  hardware->display.fillRect(0,0,90,10,BLACK);
  hardware->display.print(scale->getName());
  
  
  Serial.print("end of WaveMode start(), numPatches is ");
  Serial.println(numPatches);
};

void WaveMode::stop()
{
  delete scale;
  GravitoneOutputMode::stop();
  clearPatches();
  Serial.println("stoppping WaveMode");
  hardware->display.clearDisplay();
}

void WaveMode::onUpdateOrientation() 
{
  double yaw   = hardware->getYaw();
  double pitch = hardware->getPitch();
  double roll  = hardware->getRoll();

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

void WaveMode::button4(butevent_t event){
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 4 pressed");
    playing = true;
    fade1.fadeIn(20);
  } else if( event == BUTTON_RELEASED ){
    playing = false;
    fade1.fadeOut(100);
  }  
}

void WaveMode::button5(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 5 pressed");
  }
}

void WaveMode::button6(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("button 6 pressed");
    scaleIndex = (scaleIndex + 1) % 12;
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleTypeIndex], gs_Notes[scaleIndex], 1);
    hardware->display.setCursor(0, 0);
    hardware->display.fillRect(0,0,90,10,BLACK);
    hardware->display.print(scale->getName());
  }
}

void WaveMode::button7(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("button 7 pressed");
    continuous = !continuous;
    hardware->display.fillRect(68,12,80,10,BLACK);
    hardware->display.setCursor(68,12);
    if( continuous ){
      hardware->display.print("Continuous");
    } else {
      hardware->display.print("Scale");
    }
  }
}

void WaveMode::button8(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("button 8 pressed");
    scaleTypeIndex = (scaleTypeIndex + 1) % GS_NUM_SCALE_PATTERNS;
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleTypeIndex], gs_Notes[scaleIndex], 1);
    hardware->display.setCursor(0, 0);
    hardware->display.fillRect(0,0,90,10,BLACK);
    hardware->display.print(scale->getName());
  }
}

void WaveMode::button9(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 9 pressed");
    hardware->display.fillRect(0,12, 50, 10, BLACK);
    hardware->display.setCursor(0,12);
    activeWaveform = WAVEFORM_TRIANGLE;
    waveform1.begin(activeWaveform);
    hardware->display.print("Triangle");
  }
}

void WaveMode::button10(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 10 pressed");
    hardware->display.fillRect(0,12, 50, 10, BLACK);
    hardware->display.setCursor(0,12);
    activeWaveform = WAVEFORM_SINE;
    waveform1.begin(activeWaveform);
    hardware->display.print("Sine");
  }
}

void WaveMode::button11(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 11 pressed");
    hardware->display.fillRect(0,12, 50, 10, BLACK);
    hardware->display.setCursor(0,12);
    activeWaveform = WAVEFORM_SAWTOOTH;
    waveform1.begin(activeWaveform);
    hardware->display.print("Sawtooth");
  }
}

void WaveMode::button12(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 12 pressed");
    activeWaveform = WAVEFORM_SQUARE;
    waveform1.begin(activeWaveform);
    hardware->display.fillRect(0,12, 50, 10, BLACK);
    hardware->display.setCursor(0,12);
    hardware->display.print("Square");
  }
}
