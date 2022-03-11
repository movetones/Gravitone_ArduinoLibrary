#include <Gravitone.h>
#include <WaveMode.h>
#include <SerialPassthroughMode.h>
#include <WebsocketDemoMode.h>

Gravitone g;

void setup() {
  // put your setup code here, to run once:

  AudioMemory(20);
  Wire.begin();
  Wire.setClock(800000);

  delay(1000);


  g.begin();

  WaveMode *waveMode1 = new WaveMode();
  WaveMode *waveMode2 = new WaveMode();
  SerialPassthroughMode *espMode = new SerialPassthroughMode();
  WebsocketDemoMode *wsMode = new WebsocketDemoMode();
  
  
  g.addMode(waveMode1);
  g.addMode(waveMode2);
  g.addMode(espMode);
  g.addMode(wsMode);
  
  g.setActiveMode(0);
  
  Serial.println("Started");

  
}

void loop() {
  // put your main code here, to run repeatedly:

  g.eventLoop();
}
