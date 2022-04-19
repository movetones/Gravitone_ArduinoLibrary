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

