#include "GravitoneMode.h"

void GravitoneMode::addPatch(AudioConnection *cable)
{

  Serial.print("adding connection ");
  Serial.println(numPatches);
  patchCoords[numPatches] = cable;
  numPatches++;  
}

void GravitoneMode::reconnectPatches()
{
  for( int i=0; i<numPatches; i++ ){
    Serial.print("reconnecting patch ");
    Serial.println(i);
    patchCoords[i]->connect();
  }
}

void GravitoneMode::disconnectPatches()
{
  #ifdef DEBUG_VERBOSE
  SERIAL.println("clearing patchCoords");
  #endif

  for( int i=0; i<numPatches; i++ ){
    Serial.print("disconnecting patch ");
    Serial.println(i);
    patchCoords[i]->disconnect();
  }
}

