#include "GravitoneMode.h"

void GravitoneMode::addPatch(AudioConnection *cable)
{
  patchCoords[numPatches] = cable;
  numPatches++;
  
  Serial.println("added connection");
  
}

void GravitoneMode::clearPatches()
{
  #ifdef DEBUG_VERBOSE
  SERIAL.println("clearing patchCoords");
  #endif

  while(numPatches > 0) {
    Serial.print("deleting patch cord ");
    Serial.println(numPatches-1);
    delete patchCoords[numPatches-1];
    numPatches--;
  }
}

