#include <Gravitone.h>
#include <WaveMode.h>

Gravitone g;

void setup() {
  // put your setup code here, to run once:

  AudioMemory(20);
  Wire.begin();
  Wire.setClock(800000);

  delay(1000);


  g.begin();

  WaveMode *mode = new WaveMode();
  g.setMode(mode);
  
  Serial.println("Started");

  
}

void loop() {
  // put your main code here, to run repeatedly:

  g.eventLoop();
}
