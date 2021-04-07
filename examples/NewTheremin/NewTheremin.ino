// Gravitone Example Configuration: With Chords
// Matt Ruffner 2019
// MoveTones LLC

#include <Audio.h>
#include <Gravitone.h>
#include <scale.h>

#define NOTE_UPDATE_INTERVAL 5

Gravitone g;


gs_Scale *scale;
uint8_t scaleIndex = 0;
uint8_t scaleTypeIndex = 5;
int octaveShift = 0;
unsigned long lastDispUpdate = 0, lastBatteryUpdate = 0, lastNoteUpdate = 0;
float roll, pitch, yaw;
float freq = 0;
int note = 0;
bool playing = false;
bool continuous = false;
int activeWaveform;

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform1;      //xy=140,403
AudioAmplifier           amp1;           //xy=323,403
AudioEffectFade          fade1;          //xy=492,402
AudioOutputI2S           i2s1;           //xy=793,403
AudioConnection          patchCord1(waveform1, amp1);
AudioConnection          patchCord2(amp1, fade1);
AudioConnection          patchCord3(fade1, 0, i2s1, 0);
AudioConnection          patchCord4(fade1, 0, i2s1, 1);
// GUItool: end automatically generated code



/////////////////////////////////////////////////////////////////////////////////////
// MAIN SETUP FUNCTION
/////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(1000);
  AudioMemory(20);


  Wire.begin();
  Wire.setClock(400000);

  Serial.println("starting gravitone...");
  if( g.begin() ){
    Serial.println("success");
  } else {
    Serial.println("fail");
  }
  delay(2000);


  waveform1.begin(0.8, 220, WAVEFORM_SINE);


  scale = new gs_Scale(GS_SCALE_PENT_MAJOR_PATTERN, gs_Notes[scaleIndex], 1);

  
  Serial.println("Setting volume to 1");
  setVolume(1);
  fade1.fadeOut(500);
  Serial.print("Started amplifier");
}

void loop() {
  unsigned long now = millis();
  
  g.updateButtons();
  if( g.buttonsAvailable() ){
    int bid = g.lastButtonPress();
    doButtonAction(bid, g.getButtonState(bid));
    if( bid >=0 ){
      Serial.print("button");
      Serial.println(bid);
    }
  }

  bool newpos = g.updateOrientation();
  if( newpos ){
    roll   = g.getRoll();
    pitch = g.getPitch();
    yaw = g.getYaw();

    String dataString = "y"+ String(yaw) + "yp" + String(pitch) + "pr" + String(roll)+"r\r\n";
    Serial.write(dataString.c_str());
    Serial.flush();

  }
  

  if( millis() - lastNoteUpdate > NOTE_UPDATE_INTERVAL ){
    
    note = map(pitch, -70, 70, 0, scale->getNoteCount());
    freq = scale->getNote(note)->freq * pow(2, octaveShift);


    // continuous tone
    if( continuous )
      freq = scale->unison.freq*pow(2,(float)(map(pitch, -60, 60, 0, 12))/12.0)*pow(2, octaveShift);
    //else 
     waveform1.frequency(freq);
  }
  
  // NOTE DISPLAY UPDATE
  if ( millis() - lastDispUpdate > 300 ) {
    //display.clearDisplay();
    g.display.setTextSize(0);
    g.display.setTextColor(WHITE, BLACK);
    g.display.setCursor(0, 0);
    g.display.fillRect(0,0,90,10,BLACK);
    g.display.print(scale->getName());
    g.display.fillRect(68,12,80,10,BLACK);
    g.display.setCursor(68,12);
    if( continuous ){
      g.display.print("Continuous");
    } else {
      g.display.print("Scale");
    }

    g.display.fillRect(0,12, 50, 10, BLACK);
    g.display.setCursor(0,12);
    switch( activeWaveform ){
      case WAVEFORM_SINE:
        g.display.print("Sine");
        break;
      case WAVEFORM_SQUARE:
        g.display.print("Square");
        break;
      case WAVEFORM_TRIANGLE:
        g.display.print("Triangle");
        break;
      case WAVEFORM_SAWTOOTH:
        g.display.print("Sawtooth");
        break;
      case WAVEFORM_PULSE:
        g.display.print("Pulse");
        break;
    }

//    g.display.setCursor(100,22);
//    g.display.fillRect(100,22,20,10,BLACK);
//    g.display.print(note); //display.print("  "); display.print(freq); display.println(" Hz ");
    g.display.display();
    lastDispUpdate = millis();
  }


  if( millis() - lastBatteryUpdate > 1000 ){
    g.updateBattery();
    lastBatteryUpdate = millis();
  }
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
//
// BUTTONS
//
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

void doButtonAction(int id, bool val) {
  static bool playPressed = false, playLock = false;
  static bool s3pressed = false, s2pressed = false, s4pressed = false, s5pressed = false, s6pressed = false, s7pressed = false, s8pressed = false, s9pressed = false, s10pressed = false;
  static bool s11pressed = false, s12pressed = false;
  static int holdCount = 0;
  switch ( id ) {
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    case 2:
      if ( val && !playing  ) {
        playPressed = true;
        playing = true;
        fade1.fadeIn(20);
      } else if (!val && playing) {
        if( !playLock ){
          playPressed = false;
          playing = false;
          fade1.fadeOut(100);
        }
      }
      break;
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    case 1:
      if( val) octaveShift += 1;
      break;

    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    case 0:
      if( val ) octaveShift -= 1;
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 3:
      if( val && !s4pressed ){
        s4pressed = true;
        activeWaveform = WAVEFORM_SINE;
        waveform1.begin(activeWaveform);
      } else if( !val && s4pressed ){
        s4pressed = false;
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 4:
      if( val && !s5pressed ){
        s5pressed = true;
        activeWaveform = WAVEFORM_SAWTOOTH;
        waveform1.begin(activeWaveform);
      } else if( !val && s5pressed ){
        s5pressed = false;
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 5:
      if( val && !s6pressed ){
        s6pressed = true;
        activeWaveform = WAVEFORM_SQUARE;
        waveform1.begin(activeWaveform);
      } else if( !val && s6pressed ){
        s6pressed = false;
      }
      break;  
//////////////////////////////////////
//////////////////////////////////////
    case 6:
      if( val && !s7pressed ){
        s7pressed = true;
        activeWaveform = WAVEFORM_TRIANGLE;
        waveform1.begin(activeWaveform);
      } else if( !val && s7pressed ){
        s7pressed = false;
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 7:
      if( val && !s8pressed ){
        s8pressed = true;
        activeWaveform = WAVEFORM_PULSE;
        waveform1.begin(activeWaveform);
      } else if( !val && s8pressed ){
        s8pressed = false;
        //strum_up(chord, 0.8);
      }
      break;   
//////////////////////////////////////
//////////////////////////////////////
    case 8:
      if( val && !s10pressed ){
        s10pressed = true;
        continuous = !continuous;
      } else if( !val && s10pressed ){
        s10pressed = false;
      }
      break;         
//////////////////////////////////////
//////////////////////////////////////
    case 9:
      if( val ){
        scaleIndex = (scaleIndex + 1) % 12;
        regenerateScale(scaleTypeIndex, scaleIndex);
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 10:
      if( val ){
        scaleTypeIndex = (scaleTypeIndex + 1) % GS_NUM_SCALE_PATTERNS;
        regenerateScale(scaleTypeIndex, scaleIndex);
      }
      break;
//////////////////////////////////////
//////////////////////////////////////
    case 11:
      if ( val && !s2pressed ) {
        cycleVolume();
        s2pressed = true;
      } else if ( !val && s2pressed ) {
        s2pressed = false;
      }
      break;            
  }
}

void regenerateScale(int  scaleType, int startNote) {
  if ( scale ) {
    delete scale;
    scale = new gs_Scale(gs_scalePatterns[scaleType], gs_Notes[startNote], 1);
  }
}

void cycleVolume() {
  cycleVolume(false);
}

void cycleVolume(bool mute) {
  static uint8_t vol = 1;
  if ( mute ) {
    setVolume(0);
  } else {
    vol = (vol + 1) % 5;
    setVolume(vol);
  }
}

void setVolume(uint8_t vol) {
  if ( vol > 4 ) {
    vol = 0;
  } else if ( vol > 0 ) {
    g.setSpeakerState(true);
    switch ( vol ) {
      case 1: 
        amp1.gain(0.2);
        break;
      case 2: 
        amp1.gain(0.5);
        break;
      case 3: 
        amp1.gain(0.7);
        break;
      case 4: 
        amp1.gain(0.9);
        break;
    }
  } else {
    g.setSpeakerState(false);
  }
  g.updateVolume((float)(vol/5.0));
}
