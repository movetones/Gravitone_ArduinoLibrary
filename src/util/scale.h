#ifndef SCALE_H
#define SCALE_H

//----------------------------------------------------------------------------------
// new scale generation scheme, msb is first note
// starting to implement new gs namespace

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

const char *gs_NoteNames[] = {
  "C",
  "C#",
  "D",
  "D#",
  "E",
  "F",
  "F#",
  "G",
  "G#",
  "A",
  "A#",
  "B"
};

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

static PROGMEM uint16_t gs_scalePatterns[] = {
  GS_SCALE_MAJOR_PATTERN,
  GS_SCALE_MINOR_PATTERN,
  GS_SCALE_HARM_MINOR_PATTERN,
  GS_SCALE_MELO_MINOR_PATTERN,
  GS_SCALE_PENT_MAJOR_PATTERN,
  GS_SCALE_PENT_MINOR_PATTERN,
  GS_SCALE_MINOR_BLUES_PATTERN
};

static PROGMEM char* gs_scalePatternNames[] = {
  GS_SCALE_MAJOR_PATTERN_NAME,
  GS_SCALE_MINOR_PATTERN_NAME,
  GS_SCALE_HARM_MINOR_PATTERN_NAME,
  GS_SCALE_MELO_MINOR_PATTERN_NAME,
  GS_SCALE_PENT_MAJOR_PATTERN_NAME,
  GS_SCALE_PENT_MINOR_PATTERN_NAME,
  GS_SCALE_MINOR_BLUES_PATTERN_NAME
};

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
  
const PROGMEM uint16_t gs_intervalPositions[] = {
  GS_UNISON, 
  GS_MINOR_SECOND, 
  GS_MAJOR_SECOND, 
  GS_MINOR_THIRD, 
  GS_MAJOR_THIRD, 
  GS_FOURTH,
  GS_TRITONE, 
  GS_FIFTH,
  GS_MINOR_SIXTH,
  GS_MAJOR_SIXTH,
  GS_MINOR_SEVENTH,
  GS_MAJOR_SEVENTH,
  GS_OCTAVE
};

const PROGMEM char * gs_intervalName[] = {
  GS_UNISON_NAME, 
  GS_MINOR_SECOND_NAME, 
  GS_MAJOR_SECOND_NAME, 
  GS_MINOR_THIRD_NAME, 
  GS_MAJOR_THIRD_NAME, 
  GS_FOURTH_NAME,
  GS_TRITONE_NAME, 
  GS_FIFTH_NAME,
  GS_MINOR_SIXTH_NAME,
  GS_MAJOR_SIXTH_NAME,
  GS_MINOR_SEVENTH_NAME,
  GS_MAJOR_SEVENTH_NAME,
  GS_OCTAVE_NAME
};

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

const PROGMEM uint16_t gs_chordPatterns[GS_NUM_CHORD_PATTERNS] = {
  GS_MAJ_CHORD_PATTERN,
  GS_MIN_CHORD_PATTERN,
  GS_DIM_CHORD_PATTERN,
  GS_MAJ7_CHORD_PATTERN,
  GS_DOM7_CHORD_PATTERN,
  GS_MIN7_CHORD_PATTERN,
  GS_MIN7_FLA5_CHORD_PATTERN
};

const PROGMEM char* gs_chordPatternNames[GS_NUM_CHORD_PATTERNS] = {
  GS_MAJ_CHORD_PATTERN_NAME_SHORT,
  GS_MIN_CHORD_PATTERN_NAME_SHORT,
  GS_DIM_CHORD_PATTERN_NAME_SHORT,
  GS_MAJ7_CHORD_PATTERN_NAME_SHORT,
  GS_DOM7_CHORD_PATTERN_NAME_SHORT,
  GS_MIN7_CHORD_PATTERN_NAME_SHORT,
  GS_MIN7_FLA5_CHORD_PATTERN_NAME_SHORT
};


class gs_NoteSet {
public:
  gs_NoteSet() : notes(NULL), noteCount(0), name("") {;}
  ~gs_NoteSet() { delete notes;}
  GS_NOTE *getNotes(){ if(noteCount) return notes; }
  int getNoteCount() { return noteCount; }
  void setNoteCount(int ct) { noteCount = ct; notes = new GS_NOTE[noteCount];}
  String getName() { return name; }
  void setName(String &n) { name = n; }
  GS_NOTE *getNote(int pos) { 
    if( notes != NULL){
      if( noteCount && pos<noteCount ){
        return &notes[pos]; 
      } else {
        //Serial.println("FUCKin too many or zero notes");
		return &notes[noteCount-1];
      }
    } else {
      //Serial.println("NULL notes");
    }
    return NULL;
  }
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
  explicit gs_Chord(uint16_t pattern, GS_NOTE unisonNote) : gs_NoteSet() {
    uint16_t cp = pattern;
    int a = ((cp>>15)&1)+((cp>>14)&1)+((cp>>13)&1)+((cp>>12)&1);
    a    += ((cp>>11)&1)+((cp>>10)&1)+((cp>>9)&1)+((cp>>8)&1);
    a    += ((cp>>7)&1)+((cp>>6)&1)+((cp>>5)&1)+((cp>>4)&1);
    a    += ((cp>>3)&1)+((cp>>2)&1)+((cp>>1)&1)+((cp)&1);
    setNoteCount(a);
    chordPattern = pattern;

    for( int i=0;i<GS_NUM_CHORD_PATTERNS;i++ ){
      if( gs_chordPatterns[i]==chordPattern ){
        String n(gs_NoteNames[unisonNote.pos]);
        n.append(" ");
        n.append(gs_chordPatternNames[i]);
        setName(n);
        break;
      }
    }
    
    int nd = 0, ni=0;
    
    for( uint16_t testBit=0x8000; testBit >= 0x0008; testBit>>=1, ni++ ){
      if( chordPattern & testBit ){
        float f = unisonNote.freq * pow(2,(float)ni/12.0);
        int p = (unisonNote.pos + ni) % 12; 
        setNote(nd, f, p);
        nd++;
      }
    }
  }
  ~gs_Chord() {;}
private:
  uint16_t chordPattern;
};


class gs_Scale : public gs_NoteSet{
public:
  gs_Scale(uint16_t pattern, GS_NOTE unisonNote, int numOct) : gs_NoteSet() {
    uint16_t cp = pattern;
    int a = ((cp>>15)&1)+((cp>>14)&1)+((cp>>13)&1)+((cp>>12)&1);
    a    += ((cp>>11)&1)+((cp>>10)&1)+((cp>>9)&1)+((cp>>8)&1);
    a    += ((cp>>7)&1)+((cp>>6)&1)+((cp>>5)&1)+((cp>>4)&1);
    a    += ((cp>>3)&1)+((cp>>2)&1)+((cp>>1)&1)+((cp)&1);
    
    setNoteCount(a * numOct - (numOct>1 ? numOct-1 : 0));
    numOctaves = numOct;
    scalePattern = pattern;
	unison = unisonNote;

    for( int i=0;i<GS_NUM_SCALE_PATTERNS;i++ ){
      if( gs_scalePatterns[i]==scalePattern ){
        String n(gs_NoteNames[unisonNote.pos]);
        n.append(" ");
        n.append(gs_scalePatternNames[i]);
        setName(n);
      }
    }

    int nd = 0;

    for( int i=0; i<numOctaves; i++ ){
      int ni = (i==0 ? 0 : 1);
      for( uint16_t testBit = (i==0 ? 0x8000 : 0x4000); testBit >= 0x0008; testBit>>=1, ni++ ){
        if( scalePattern & testBit ){
          float f = pow(2,i) * unisonNote.freq * pow(2,(float)ni/12.0);
          int p = (unisonNote.pos + ni) % 12;   
          setNote(nd, f, p);
          nd++;
        }
      }
    }
  } 
  ~gs_Scale() {;}
  uint8_t getUnisonPosition() {
	  return unison.pos;
  }
  uint8_t getPositionFromNoteNumber(uint8_t n){
	uint8_t nn = 0;
	//Serial.print("in getPositionFromNoteNumber, n="); Serial.println(n);
	for( int i=0; i<numOctaves; i++ ){
		int ni = 0;//(i==0 ? 0 : 1);
		
		for( uint16_t testBit = (i==0 ? 0x8000 : 0x4000); testBit >= 0x0008; testBit>>=1, ni++ ){
			if( nn==n ){
				return ni;
			}
			if( scalePattern & testBit ){
			  nn++;
			}
			//Serial.print("inner loop, ni="); Serial.print(ni); Serial.print(" , nn="); Serial.println(nn);
		}
	}
  }
public:
  GS_NOTE unison;
private:
  int numOctaves;
  uint16_t scalePattern;
};

#endif
