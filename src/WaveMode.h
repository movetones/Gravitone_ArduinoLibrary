#include "Gravitone.h"


class WaveMode : public GravitoneMode {
public:

  WaveMode();
  ~WaveMode();
  
  void start(gtdisplay_t & display);
  
  void button4(butevent_t event, gtdisplay_t & display);
  void button5(butevent_t event, gtdisplay_t & display);
  void button6(butevent_t event, gtdisplay_t & display);
  void button7(butevent_t event, gtdisplay_t & display);
  void button8(butevent_t event, gtdisplay_t & display);
  void button9(butevent_t event, gtdisplay_t & display);
  void button10(butevent_t event, gtdisplay_t & display);
  void button11(butevent_t event, gtdisplay_t & display);
  void button12(butevent_t event, gtdisplay_t & display);
  
  void onUpdateOrientation(double y, double p, double r);
  
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


