/* Gravitone Library
 * Copyright (c) 2022, Matt Ruffner <matthew.ruffner@movetones.com>
 * MoveTones, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


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
