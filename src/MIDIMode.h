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

#include "GravitoneMode.h"
#include "scale.h"

/*
 *  @brief MIDIMode - output midi messages in response to button presses
 *  and orientation changes of the gravitone
 */

class MIDIMode : public GravitoneMode {
public:

  MIDIMode();
  ~MIDIMode();
  
  void start();
  void stop();
  
  void button2(butevent_t event);
  void button3(butevent_t event);
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

  float s1,s2,s3;
  uint8_t page;
  int channel;

private:
  bool b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12;

};


