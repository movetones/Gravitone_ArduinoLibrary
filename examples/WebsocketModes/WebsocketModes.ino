#include <Gravitone.h>
#include <WaveMode.h>
#include <ScaleMode.h>
#include <SerialPassthroughMode.h>
#include <WebsocketDemoMode.h>

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
  SerialPassthroughMode *espMode = new SerialPassthroughMode();
  WebsocketDemoMode *wsMode = new WebsocketDemoMode();
  
  
  g.addMode(scaleMode);
  g.addMode(waveMode);
  g.addMode(espMode);
  g.addMode(wsMode);
  
  g.setActiveMode(0);
  
  Serial.println("Started");

  
}

void loop() {
  // put your main code here, to run repeatedly:

  g.eventLoop();
}
