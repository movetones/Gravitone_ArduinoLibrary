#include "GravitoneOutputMode.h"
#include "scale.h"

class WaveMode : public GravitoneOutputMode {
public:

  WaveMode();
  ~WaveMode();
  
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
  float freq, freq2, freq3;
  float baseNote;
  int octaveShift;

  AudioSynthWaveform       waveform1;      //xy=140,403
  AudioSynthWaveform       waveform2;      //xy=140,403
  AudioSynthWaveform       waveform3;      //xy=140,403
  AudioEffectFade          fade1;          //xy=492,402
  AudioEffectFade          fade2;          //xy=492,402
  AudioEffectFade          fade3;          //xy=492,402
  AudioMixer4              mixer1;
};


