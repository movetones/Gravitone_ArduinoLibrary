#include "ModeXylotar.h"

ModeXylotar::ModeXylotar()
{
  idleThresh = 1;
  trigThresh = 2.1;
  playChord = true;
  lastSampleTime = 0;
  lastPowerValue = 0;
  scale = new gs_Scale(GS_SCALE_MAJOR_PATTERN, gs_Notes[NOTE_G.pos], 3);
  chord = new gs_Chord(GS_MAJ_CHORD_PATTERN, gs_Notes[NOTE_G.pos]);

  sine1.begin(0.8, 440, WAVEFORM_SAWTOOTH);
  sine2.begin(0.5, 440, WAVEFORM_SAWTOOTH);
  sine3.begin(0.5, 440, WAVEFORM_SAWTOOTH);
  sine4.begin(0.4, 440, WAVEFORM_SAWTOOTH);

  mixer1.gain(0,0.9);
  mixer1.gain(1,0.9);
  mixer1.gain(2,0.9);
  mixer1.gain(3,0.9);

  envelope1.delay(0);
  envelope1.hold(0);
  envelope2.delay(0);
  envelope2.hold(0);
  envelope3.delay(0);
  envelope3.hold(0);
  envelope4.delay(0);
  envelope4.hold(0);

  addPatch( new AudioConnection(sine1, 0, envelope1, 0));
  addPatch( new AudioConnection(sine2, 0, envelope2, 0));
  addPatch( new AudioConnection(sine3, 0, envelope3, 0));
  addPatch( new AudioConnection(sine4, 0, envelope4, 0));

  addPatch( new AudioConnection(envelope1, 0, mixer1, 0));
  addPatch( new AudioConnection(envelope2, 0, mixer1, 1));
  addPatch( new AudioConnection(envelope3, 0, mixer1, 2));
  addPatch( new AudioConnection(envelope4, 0, mixer1, 3));
  addPatch( new AudioConnection(mixer1, 0, amp1, 0));
}

void ModeXylotar::start()
{
  GravitoneOutputMode::start();

}

void ModeXylotar::stop()
{
  GravitoneOutputMode::stop();

}

const char * ModeXylotar::getName()
{
  return "ModeXylotar";
}

void ModeXylotar::onUpdateDisplay()
{
  hardware->display.setTextSize(0);
  hardware->display.setTextColor(WHITE, BLACK);
  hardware->display.setCursor(0, 0);
  hardware->display.print("Xylotar");

}

void ModeXylotar::onUpdateOrientation()
{
  auto acc = hardware->getAcc();
  auto R = hardware->getRot();

  auto accr = R * acc;

  static int noteIndex = 0;
  static unsigned long lastPeakTime = 0;

  int trig = 0;
  double vel = 0.0;

  float power = sqrt(acc(2)*acc(2) + acc(1)*acc(1));

  power = max(0, min( 5, power));

  // looking for peak
  if( peakState == 0 ){
    if ( power > trigThresh ){

      noteIndex = map(hardware->getHeading(), 180, -180, 0, scale->getNoteCount());

      vel = map(power, trigThresh, 5, 0.5, 1.0);

      trig = 1;

      freq = scale->getNote(noteIndex)->freq;

      getNotesOfChord(chordType, freq, freqs);

      //if( freqs[0] > 0)
      sine1.frequency(freqs[0]);
      envelope1.noteOn();

      if( playChord ){
        if( freqs[1] > 0) {
          sine2.frequency(freqs[1]);
          envelope2.noteOn();
        }
        if( freqs[2] > 0) {
          sine3.frequency(freqs[2]);
          envelope3.noteOn();
        }
        if( freqs[3] > 0) {
          sine4.frequency(freqs[3]);
          envelope4.noteOn();
        }
      }

      peakState = 1; // wait for power to fall below idle thresh
    } else {
      trig = 0;
    }
  }
  else if( peakState == 1 ) {
    if( power < idleThresh ){
      peakState = 0;
      envelope1.noteOff();
      envelope2.noteOff();
      envelope3.noteOff();
      envelope4.noteOff();
    }
  }


  Serial.print(freqs[0]);
  Serial.print(",");
  Serial.print(freqs[1]);
  Serial.print(",");
  Serial.print(freqs[2]);
  Serial.print(",");
  Serial.println(freqs[3]);

}

/**
 * @brief ModeXylotar::getNoteOfChord
 * @param cp the chord pattern to use (defined in scale.h)
 * @param baseNote the frequency of the 1 note
 * @param pos index of the note you want, if this is more than the numbe of
 * notes in the chord, a number less than 0  is returned
 * @return
 */
float ModeXylotar::getNotesOfChord(uint16_t cp, float baseNote, float* output)
{
  // calculate the output frequencies
  int ni = 0, nd=0;
  for( uint16_t testBit = 0x8000; testBit >= 0x0008; testBit>>=1, ni++ ){
    if( cp & testBit ){
      output[nd] = baseNote * pow(2,(float)ni/12.0);
      nd++;
    }
  }

  // zero the rest of the output frequencies
  while( nd < 6 ){
    output[nd] = 0.0;
    nd++;
  }
}

void ModeXylotar::button4(butevent_t event)
{
  if( event == BUTTON_PRESSED ){
    chordType = GS_MAJ_CHORD_PATTERN;
    playChord = true;
  } else if( event == BUTTON_RELEASED ){
    playChord = false;
  }
}

void ModeXylotar::button5(butevent_t event)
{
  if( event == BUTTON_PRESSED ){
    chordType = GS_MIN_CHORD_PATTERN;
    playChord = true;
  } else if( event == BUTTON_RELEASED ){
    playChord = false;
  }
}

void ModeXylotar::button6(butevent_t event)
{
  if( event == BUTTON_PRESSED ){
    chordType = GS_MAJ7_CHORD_PATTERN;
    playChord = true;
  } else if( event == BUTTON_RELEASED ){
    playChord = false;
  }
}

void ModeXylotar::button7(butevent_t event)
{
  if( event == BUTTON_PRESSED ){
    chordType = GS_DOM7_CHORD_PATTERN;
    playChord = true;
  } else if( event == BUTTON_RELEASED ){
    playChord = false;
  }
}

void ModeXylotar::button8(butevent_t event)
{
  if( event == BUTTON_PRESSED ){
    chordType = GS_MIN7_CHORD_PATTERN;
    playChord = true;
  } else if( event == BUTTON_RELEASED ){
    playChord = false;
  }
}

void ModeXylotar::button9(butevent_t event)
{
  if( event == BUTTON_PRESSED ){
    chordType = GS_DIM_CHORD_PATTERN;
    playChord = true;
  } else if( event == BUTTON_RELEASED ){
    playChord = false;
  }
}

void ModeXylotar::button10(butevent_t event)
{
  if( event == BUTTON_PRESSED ){
    chordType = GS_MIN7_FLA5_CHORD_PATTERN;
    playChord = true;
  } else if( event == BUTTON_RELEASED ){
    playChord = false;
  }
}

void ModeXylotar::button11(butevent_t event)
{

}

void ModeXylotar::button12(butevent_t event)
{

}
