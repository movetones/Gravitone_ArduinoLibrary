/*
 * Modes example for the Gravitone
 * 
 * This examples shows the steps necessary to create several 
 * modes and install them to the Gravitone object.
 * 
 * Matt Ruffner 2022
 * MoveTones, LLC
 */

#include <Gravitone.h>
#include <WaveMode.h>
#include <ScaleMode.h>
#include <ModeXylotar.h>
#include <QuaternionOutputMode.h>

Gravitone g;

void setup() {
  // put your setup code here, to run once:

  AudioMemory(50);
  Wire.begin();
  Wire.setClock(800000);

  delay(1000);


  g.begin();

  ScaleMode *scaleMode = new ScaleMode();
  WaveMode *waveMode = new WaveMode();
  ModeXylotar *xylotarMode = new ModeXylotar();
  QuaternionOutputMode *quatOutput = new QuaternionOutputMode();
  
  g.addMode(scaleMode);
  g.addMode(waveMode);
  g.addMode(xylotarMode);
  g.addMode(quatOutput);
  
  g.setActiveMode(0);
  
  Serial.println("Started");

  
}

void loop() {
  // put your main code here, to run repeatedly:

  g.eventLoop();
}
