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
  
  const char * getName();
  
  int activeWaveform;
  bool playing;
  bool continuous;
  int note;
  float freq;
  int octaveShift;
  uint8_t scaleTypeIndex;  
  uint8_t scaleIndex;
  gs_Scale *scale;

  AudioSynthWaveform       waveform1;      //xy=140,403
};


