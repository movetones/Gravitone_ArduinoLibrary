#include "scale.h"


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







static PROGMEM char* gs_scalePatternNames[] = {
  GS_SCALE_MAJOR_PATTERN_NAME,
  GS_SCALE_MINOR_PATTERN_NAME,
  GS_SCALE_HARM_MINOR_PATTERN_NAME,
  GS_SCALE_MELO_MINOR_PATTERN_NAME,
  GS_SCALE_PENT_MAJOR_PATTERN_NAME,
  GS_SCALE_PENT_MINOR_PATTERN_NAME,
  GS_SCALE_MINOR_BLUES_PATTERN_NAME
};


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


GS_NOTE* gs_NoteSet::getNote(int pos) { 
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


gs_Chord::gs_Chord(uint16_t pattern, GS_NOTE unisonNote) : gs_NoteSet() {
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
  




gs_Scale::gs_Scale(uint16_t pattern, GS_NOTE unisonNote, int numOct) : gs_NoteSet() {

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

uint8_t gs_Scale::getPositionFromNoteNumber(uint8_t n){
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

