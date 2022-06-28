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

#include "MIDIMode.h"

#define CONTROL_INDEX( page , id) ( page * 9 + id )

MIDIMode::MIDIMode() : channel(0),page(0) {
}

MIDIMode::~MIDIMode() {

} 

void MIDIMode::start() {
  Serial.println("Starting MIDI mode!");

}
void MIDIMode::stop() {
  Serial.println("Stopping MIDI Mode!");
}

void MIDIMode::button2(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    page--;
  } else if( event == BUTTON_RELEASED) {
    
  }
}

void MIDIMode::button3(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    page++;
  } else if( event == BUTTON_RELEASED) {

  }
}

void MIDIMode::button4(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b4 = true;
  } else if( event == BUTTON_RELEASED) {
    b4 = false;
  }
}

void MIDIMode::button5(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b5 = true;
  } else if( event == BUTTON_RELEASED) {
    b5 = false;
  }
}

void MIDIMode::button6(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b6 = true;
  } else if( event == BUTTON_RELEASED) {
    b6 = false;
  }
}

void MIDIMode::button7(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b7 = true;
  } else if( event == BUTTON_RELEASED) {
    b7 = false;
  }
}

void MIDIMode::button8(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b8 = true;
  } else if( event == BUTTON_RELEASED) {
    b8 = false;
  }
}
void MIDIMode::button9(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b9 = true;
  } else if( event == BUTTON_RELEASED) {
    b9 = false;
  }
}

void MIDIMode::button10(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b10 = true;
  } else if( event == BUTTON_RELEASED) {
    b10 = false;
  }
}

void MIDIMode::button11(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b11 = true;
  } else if( event == BUTTON_RELEASED) {
    b11 = false;
  }
}

void MIDIMode::button12(butevent_t event) {
  if( event == BUTTON_PRESSED ){
    b12 = true;
  } else if( event == BUTTON_RELEASED) {
    b12 = false;
  }
}

void MIDIMode::onUpdateOrientation() {

  if( b4 ){
    s1 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 0 ), s1, channel);
  }

  if( b5 ){
    s1 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 1 ), s1, channel);
  }

  if( b6 ){
    s1 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 2 ), s1, channel);
  }

  if( b7 ){
    s1 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 3 ), s1, channel);
  }

  if( b8 ){
    s1 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 4 ), s1, channel);
  }

  if( b9 ){
    s2 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 5 ), s2, channel);
  }

  if( b10 ){
    s2 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 6 ), s2, channel);
  }

  if( b11 ){
    s3 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 7 ), s3, channel);
  }

  if( b12 ){
    s2 = map(hardware->getRoll(), 0, 180, 0, 127);
    usbMIDI.sendControlChange(CONTROL_INDEX(page, 8 ), s2, channel);
  }

   // MIDI Controllers should discard incoming MIDI messages.
  while (usbMIDI.read()) {
  }
}

void MIDIMode::onUpdateDisplay() {
  hardware->display.setTextSize(0);
  hardware->display.setTextColor(WHITE, BLACK);
  hardware->display.setCursor(0, 0);
  hardware->display.print("MIDI Mode");
  hardware->display.setCursor(0,12);
  hardware->display.print("Page: ");
  hardware->display.print(page);

}

const char * MIDIMode::getName(){
  return "MIDI Mode";
}