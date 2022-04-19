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



#ifndef SCALE_H
#define SCALE_H

#include <Arduino.h>



//----------------------------------------------------------------------------------
// new scale generation scheme, msb is first note
// starting to implement new gs namespace

// DIFFERENT TYPES OF SCALE PATTERNS
#define GS_NUM_SCALE_PATTERNS       7

#define GS_SCALE_MAJOR_PATTERN      0xAD58 // 1010110101011XXX
#define GS_SCALE_MINOR_PATTERN      0xB5A8 // 1011010110101XXX
#define GS_SCALE_HARM_MINOR_PATTERN 0xB598 // 1011010110011XXX
#define GS_SCALE_MELO_MINOR_PATTERN 0xB558 // 1011010101011XXX
#define GS_SCALE_PENT_MAJOR_PATTERN 0xA948 // 1010100101001XXX
#define GS_SCALE_PENT_MINOR_PATTERN 0x9528 // 1001010100101XXX
#define GS_SCALE_MINOR_BLUES_PATTERN 0x9728 // 1001011100101XXX

// SCALE PATTERN NAMES
#define GS_SCALE_MAJOR_PATTERN_NAME         "Major"
#define GS_SCALE_MINOR_PATTERN_NAME         "Minor"
#define GS_SCALE_HARM_MINOR_PATTERN_NAME    "Har. Minor"
#define GS_SCALE_MELO_MINOR_PATTERN_NAME    "Mel. Minor"
#define GS_SCALE_PENT_MAJOR_PATTERN_NAME    "Pen. Major"
#define GS_SCALE_PENT_MINOR_PATTERN_NAME    "Pen. Minor"
#define GS_SCALE_MINOR_BLUES_PATTERN_NAME    "Blues"

//#define GS_SCALE_USER_SCALE_PATTERN 0xFFF8 // 1111111111111XXX
//#define GS_SCALE_USER_SCALE_PATTERN_NAME    "Chromatic"


// EQUAL TEMPERMENT POSITIONS AND NAMES
#define GS_UNISON               0x8000    
#define GS_UNISON_NAME          "Unison"
#define GS_MINOR_SECOND         0x4000    
#define GS_MINOR_SECOND_NAME    "Minor Second"
#define GS_MAJOR_SECOND         0x2000    
#define GS_MAJOR_SECOND_NAME    "Major Second"
#define GS_MINOR_THIRD          0x1000    
#define GS_MINOR_THIRD_NAME     "Minor Third"
#define GS_MAJOR_THIRD          0x0800    
#define GS_MAJOR_THIRD_NAME     "Major Third"
#define GS_FOURTH               0x0400    
#define GS_FOURTH_NAME          "Fourth"
#define GS_TRITONE              0x0200    
#define GS_TRITONE_NAME         "Tritone"
#define GS_FIFTH                0x0100    
#define GS_FIFTH_NAME           "Fifth"
#define GS_MINOR_SIXTH          0x0080    
#define GS_MINOR_SIXTH_NAME     "Minor Sixth"
#define GS_MAJOR_SIXTH          0x0040    
#define GS_MAJOR_SIXTH_NAME     "Major Sixth"
#define GS_MINOR_SEVENTH        0x0020    
#define GS_MINOR_SEVENTH_NAME   "Minor Seventh"
#define GS_MAJOR_SEVENTH        0x0010    
#define GS_MAJOR_SEVENTH_NAME   "Major Seventh"
#define GS_OCTAVE               0x0008    
#define GS_OCTAVE_NAME          "Octave"
  

// CHORD PATTERNS 
#define GS_NUM_CHORD_PATTERNS       7

#define GS_MAJ_CHORD_PATTERN        GS_UNISON | GS_MAJOR_THIRD | GS_FIFTH
#define GS_MIN_CHORD_PATTERN        GS_UNISON | GS_MINOR_THIRD | GS_FIFTH
#define GS_DIM_CHORD_PATTERN        GS_UNISON | GS_MINOR_THIRD | GS_TRITONE
#define GS_MAJ7_CHORD_PATTERN       GS_UNISON | GS_MAJOR_THIRD | GS_FIFTH | GS_MAJOR_SEVENTH
#define GS_DOM7_CHORD_PATTERN       GS_UNISON | GS_MAJOR_THIRD | GS_FIFTH | GS_MINOR_SEVENTH
#define GS_MIN7_CHORD_PATTERN       GS_UNISON | GS_MINOR_THIRD | GS_FIFTH | GS_MINOR_SEVENTH
#define GS_MIN7_FLA5_CHORD_PATTERN  GS_UNISON | GS_MINOR_THIRD | GS_TRITONE | GS_MINOR_SEVENTH

// CHORD PATTERN NAMES
#define GS_MAJ_CHORD_PATTERN_NAME         "Major"
#define GS_MIN_CHORD_PATTERN_NAME         "Minor"
#define GS_DIM_CHORD_PATTERN_NAME         "Diminished"
#define GS_MAJ7_CHORD_PATTERN_NAME        "Major 7th"
#define GS_DOM7_CHORD_PATTERN_NAME        "Dominant 7th"
#define GS_MIN7_CHORD_PATTERN_NAME        "Minor 7th"    
#define GS_MIN7_FLA5_CHORD_PATTERN_NAME   "Minor 7th Flat 5th"
// CHORD PATTERN SHORT NAMES
#define GS_MAJ_CHORD_PATTERN_NAME_SHORT         "M"
#define GS_MIN_CHORD_PATTERN_NAME_SHORT         "m"
#define GS_DIM_CHORD_PATTERN_NAME_SHORT         "Dim"
#define GS_MAJ7_CHORD_PATTERN_NAME_SHORT        "M7"
#define GS_DOM7_CHORD_PATTERN_NAME_SHORT        "Dom7"
#define GS_MIN7_CHORD_PATTERN_NAME_SHORT        "m7"    
#define GS_MIN7_FLA5_CHORD_PATTERN_NAME_SHORT   "m7b5"




/** @struct gs_note
 *
 */
typedef struct gs_note {
  float freq;
  uint8_t pos;
} GS_NOTE;

const PROGMEM GS_NOTE NOTE_C  = { .freq=261.63, .pos=0 };
const PROGMEM GS_NOTE NOTE_Cs = { .freq=277.18, .pos=1 };
const PROGMEM GS_NOTE NOTE_D  = { .freq=293.66, .pos=2 };
const PROGMEM GS_NOTE NOTE_Ds = { .freq=311.13, .pos=3 };
const PROGMEM GS_NOTE NOTE_E  = { .freq=329.63, .pos=4 };
const PROGMEM GS_NOTE NOTE_F  = { .freq=174.61, .pos=5 };
const PROGMEM GS_NOTE NOTE_Fs = { .freq=185.00, .pos=6 };
const PROGMEM GS_NOTE NOTE_G =  { .freq=196.00, .pos=7 };
const PROGMEM GS_NOTE NOTE_Gs = { .freq=207.65, .pos=8 };
const PROGMEM GS_NOTE NOTE_A =  { .freq=220.00, .pos=9 };
const PROGMEM GS_NOTE NOTE_As = { .freq=233.08, .pos=10 };
const PROGMEM GS_NOTE NOTE_B =  { .freq=246.94, .pos=11 };


const GS_NOTE gs_Notes[] = {
  NOTE_C,
  NOTE_Cs,
  NOTE_D,
  NOTE_Ds,
  NOTE_E,
  NOTE_F,
  NOTE_Fs,
  NOTE_G,
  NOTE_Gs,
  NOTE_A,
  NOTE_As,
  NOTE_B,
};

static PROGMEM uint16_t gs_scalePatterns[] = {
  GS_SCALE_MAJOR_PATTERN,
  GS_SCALE_MINOR_PATTERN,
  GS_SCALE_HARM_MINOR_PATTERN,
  GS_SCALE_MELO_MINOR_PATTERN,
  GS_SCALE_PENT_MAJOR_PATTERN,
  GS_SCALE_PENT_MINOR_PATTERN,
  GS_SCALE_MINOR_BLUES_PATTERN
};






class gs_NoteSet {
public:
  gs_NoteSet() : notes(NULL), noteCount(0), name("") {;}
  ~gs_NoteSet() { delete notes;}
  GS_NOTE *getNotes(){ if(noteCount) return notes; }
  int getNoteCount() { return noteCount; }
  void setNoteCount(int ct) { noteCount = ct; if(notes) delete notes; notes = new GS_NOTE[noteCount];}
  String getName() { return name; }
  void setName(String &n) { name = n; }
  GS_NOTE *getNote(int pos);
  void setNote(int i, float f, int p) {
    if( 0<=i && i<noteCount ){
      notes[i] = {.freq=f, .pos=p};
    }
  }
private:
  GS_NOTE *notes;
  int noteCount;
  String name;
};


class gs_Chord : public gs_NoteSet {
public:
  gs_Chord(uint16_t pattern, GS_NOTE unisonNote);
  ~gs_Chord() {;}
private:
  uint16_t chordPattern;
};


class gs_Scale : public gs_NoteSet{
public:
  gs_Scale(uint16_t pattern, GS_NOTE unisonNote, int numOct);
  ~gs_Scale() {;}
  uint8_t getUnisonPosition() {
	  return unison.pos;
  }
  uint8_t getPositionFromNoteNumber(uint8_t n);
public:
  GS_NOTE unison;
private:
  int numOctaves;
  uint16_t scalePattern;
};

#endif
