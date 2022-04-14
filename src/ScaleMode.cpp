#include "ScaleMode.h"

ScaleMode::ScaleMode() {
  scaleIndex = 0;
  scaleTypeIndex = 0;
  activeWaveform = WAVEFORM_SINE;
  octaveShift = 0;
  continuous = false;
  vibrato1 = false;
  vibrato2 = false;
  scale = new gs_Scale(GS_SCALE_PENT_MAJOR_PATTERN, gs_Notes[NOTE_G.pos], 1);
  //Serial.println("created scale");
  fade1.fadeOut(200);
  fade2.fadeOut(200);
  waveform1.begin(0.9, 440, activeWaveform);
  waveform2.begin(0.9, 220, activeWaveform);
  addPatch( new AudioConnection(waveform1, fade1) );
  addPatch( new AudioConnection(waveform2, fade2) );
  addPatch( new AudioConnection(fade1, 0, mixer1, 0) );
  addPatch( new AudioConnection(fade2, 0, mixer1, 1) );
  addPatch( new AudioConnection(mixer1, 0, amp1, 0) );
  mixer1.gain(0, 0.9);
  mixer1.gain(1, 0.9);
  //Serial.println("done with WaveMode constuctor");
};

ScaleMode::~ScaleMode() {
  delete scale;
}

const char *ScaleMode::getName()
{
  return "ScaleMode";
}

void ScaleMode::start() 
{
  GravitoneOutputMode::start();

  waveform1.amplitude(0.9);
  waveform2.amplitude(0.9);
  
  
  Serial.print("end of ScaleMode start(), numPatches is ");
  Serial.println(numPatches);
//  Serial.print(" address of scale is 0x");
//  Serial.println((unsigned long)scale, HEX);
};

void ScaleMode::stop()
{
  //waveform1.amplitude(0);
  //delete scale;
  GravitoneOutputMode::stop();
  //clearPatches();
  Serial.println("stoppping ScaleMode");
  hardware->display.clearDisplay();
}

void ScaleMode::onUpdateDisplay()
{
  hardware->display.setTextSize(0);
  hardware->display.setTextColor(WHITE, BLACK);
  hardware->display.setCursor(0, 0);
  hardware->display.print("ScaleMode");

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

  hardware->display.setCursor(0, 22);
  hardware->display.fillRect(0,22,90,10,BLACK);
  hardware->display.print(scale->getName());
}

void ScaleMode::onUpdateOrientation() 
{
  double heading = hardware->getHeading();
  double pitch = hardware->getPitch();
  double roll  = hardware->getRoll();
  static unsigned long sampleRateTrackerBuffer[50];
  double averageSampleRate = 0.0;
  static uint8_t sampleRateTrackerLoc = 0;

  bool newNote = false, newNote2 = false, newNote3 = false;

  if( map(pitch, 165, 15, 0, scale->getNoteCount()) != note ){
    note = map(pitch, 165, 15, 0, scale->getNoteCount());
    newNote = true; 
  }
  if( map(roll, 15, 165, 0, scale->getNoteCount()) != note2 ){
    note2 = map(roll, 15, 165, 0, scale->getNoteCount());
    newNote2 = true;
  }
  
  if( !vibrato1 )
    freq = scale->getNote(note)->freq * pow(2, octaveShift);
  else
    freq = scale->getNote(note)->freq * pow(2, hardware->getAx()/4.0 );

  if( !vibrato2 )
    freq2 = scale->getNote(note2)->freq * pow(2, octaveShift);
  else
    freq2 = scale->getNote(note2)->freq * pow(2, octaveShift + hardware->getAy()/4.0 );

  /*
  sampleRateTrackerBuffer[ sampleRateTrackerLoc++ ] = millis();
  if( sampleRateTrackerLoc == 50 ){
    averageSampleRate = 0.0;
    for( int i=1; i<20; i++ ){
      averageSampleRate += ( sampleRateTrackerBuffer[i] - sampleRateTrackerBuffer[i-1] );
    }
    averageSampleRate /= 20;
    averageSampleRate = 1.0 / ((double)averageSampleRate / 1000.0);
    Serial.print("Average sample rate: ");
    Serial.print(averageSampleRate);
    Serial.println(" Hz");
    sampleRateTrackerLoc = 0;
  }*/

  //Serial.print("on update orientation ");
  //Serial.print(millis()); Serial.print(" "); Serial.print(yaw); Serial.print(" "); Serial.print(pitch); Serial.print(" "); Serial.println(roll);

  if( continuous ){
    freq  = scale->unison.freq*pow(2,(float)(map(pitch, 165, 15, 0, 12))/12.0)*pow(2, octaveShift);
    freq2 = scale->unison.freq*pow(2,(float)(map(roll, 15, 165, 0, 12))/12.0)*pow(2, octaveShift);
  }

  //else 
  if( newNote )
    waveform1.frequency(freq);
  if( newNote2 )
    waveform2.frequency(freq2);
}

void ScaleMode::button4(butevent_t event){
  if( event == BUTTON_PRESSED ){
    fade1.fadeIn(20);
  } else if( event == BUTTON_RELEASED ){
    fade1.fadeOut(100);
  }  
}

void ScaleMode::button5(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    fade2.fadeIn(20);
  } else if( event == BUTTON_RELEASED ){
    fade2.fadeOut(100);
  }
}

void ScaleMode::button6(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    fade1.fadeIn(20);
    fade2.fadeIn(20);
  } else if( event == BUTTON_RELEASED) {
    fade1.fadeOut(100);
    fade2.fadeOut(100);
  }
}

void ScaleMode::button7(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    continuous = !continuous;
  }
}

void ScaleMode::button8(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    vibrato2 = true;
    fade2.fadeIn(20);
  } else if( event == BUTTON_RELEASED) {
    vibrato2 = false;
    fade2.fadeOut(100);
  }
}

void ScaleMode::button9(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    vibrato1 = true;
    fade1.fadeIn(20);
  } else if( event == BUTTON_RELEASED ){
    vibrato1 = false;
    fade1.fadeOut(100);
  }  
}

void ScaleMode::button10(butevent_t event) {
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

void ScaleMode::button11(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    scaleIndex = (scaleIndex + 1) % 12;
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleTypeIndex], gs_Notes[scaleIndex], 1);
  }
}

void ScaleMode::button12(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    scaleTypeIndex = (scaleTypeIndex + 1) % GS_NUM_SCALE_PATTERNS;
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleTypeIndex], gs_Notes[scaleIndex], 1);
  }
}
