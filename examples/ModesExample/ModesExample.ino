#include <Gravitone.h>
#include <WaveMode.h>
#include <ScaleMode.h>

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
  
  g.addMode(scaleMode);
  g.addMode(waveMode);
  
  g.setActiveMode(0);
  
  Serial.println("Started");

  
}

void loop() {
  // put your main code here, to run repeatedly:

  g.eventLoop();
}
