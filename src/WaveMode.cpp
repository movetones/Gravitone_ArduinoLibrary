#include "WaveMode.h"

WaveMode::WaveMode() {
  scaleIndex = 0;
  scaleTypeIndex = 0;
  activeWaveform = WAVEFORM_SINE;
  playing = false;
  continuous = false;
  octaveShift = 0;
  note = 440;
  int note = 5;
  scale = new gs_Scale(GS_SCALE_PENT_MAJOR_PATTERN, gs_Notes[NOTE_G.pos], 1);
  Serial.println("created scale");
  waveform1.begin(0.8, 220, activeWaveform);
  addPatch( new AudioConnection(waveform1, amp1) );
  Serial.println("done with WaveMode constuctor");
};

WaveMode::~WaveMode() {
  delete scale;
}

const char *WaveMode::getName()
{
  return "WaveMode";
}

void WaveMode::start() 
{
  GravitoneOutputMode::start();

  waveform1.amplitude(0.8);
  
  Serial.print("end of WaveMode start(), numPatches is ");
  Serial.println(numPatches);
  Serial.print(" address of scale is 0x");
  Serial.println((unsigned long)scale, HEX);
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

  hardware->display.fillRect(68,12,80,10,BLACK);
  hardware->display.setCursor(68,12);
  if( continuous ){
    hardware->display.print("Continuous");
  } else {
    hardware->display.print("Scale");
  }

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

  //hardware->display.setCursor(0, 20);
  //hardware->display.print(AudioProcessorUsageMax());

  hardware->display.setCursor(0, 0);
  hardware->display.fillRect(0,0,90,10,BLACK);
  hardware->display.print(scale->getName());
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
  }
}

void WaveMode::button7(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("button 7 pressed");
    continuous = !continuous;
  }
}

void WaveMode::button8(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("button 8 pressed");
    scaleTypeIndex = (scaleTypeIndex + 1) % GS_NUM_SCALE_PATTERNS;
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleTypeIndex], gs_Notes[scaleIndex], 1);
  }
}

void WaveMode::button9(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 9 pressed");
    activeWaveform = WAVEFORM_TRIANGLE;
    waveform1.begin(activeWaveform);
  }
}

void WaveMode::button10(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 10 pressed");
    activeWaveform = WAVEFORM_SINE;
    waveform1.begin(activeWaveform);
  }
}

void WaveMode::button11(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 11 pressed");
    activeWaveform = WAVEFORM_SAWTOOTH;
    waveform1.begin(activeWaveform);
  }
}

void WaveMode::button12(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    Serial.println("Button 12 pressed");
    activeWaveform = WAVEFORM_SQUARE;
    waveform1.begin(activeWaveform);
  }
}
