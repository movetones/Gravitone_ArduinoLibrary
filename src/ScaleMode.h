#ifndef SCALEMODE_H
#define SCALEMODE_H

#include "GravitoneOutputMode.h"
#include "scale.h"

class ScaleMode : public GravitoneOutputMode {
public:

  ScaleMode();
  ~ScaleMode();
  
  void start();
  void stop();
  
  void button4(butevent_t event);
  void button5(butevent_t event);
  void button6(butevent_t event);
  void button7(butevent_t event);
  void button8(butevent_t event);
  void button9(butevent_t event);
  void button10(butevent_t event);
  void button11(butevent_t event);
  void button12(butevent_t event);
  
  void onUpdateOrientation();
  void onUpdateDisplay();

  const char * getName();
  
  int activeWaveform;
  bool playing;
  bool continuous;
  int note, note2;
  float freq, freq2;
  int octaveShift;
  bool vibrato1, vibrato2;
  uint8_t scaleTypeIndex;  
  uint8_t scaleIndex;
  gs_Scale *scale;

  AudioSynthWaveform       waveform1;      //xy=140,403
  AudioSynthWaveform       waveform2;      //xy=140,403
  AudioEffectFade          fade1;          //xy=492,402
  AudioEffectFade          fade2;          //xy=492,402
  AudioMixer4              mixer1;
};

#endif