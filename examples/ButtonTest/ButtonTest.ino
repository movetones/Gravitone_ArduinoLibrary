#include <Wire.h>
#include <GravitoneButtonDriver.h>

GravitoneButtonDriver driver = GravitoneButtonDriver::driver;

volatile int newEvent = -1;

void setup() {


  // put your setup code here, to run once:
  Serial.begin(115200);

    delay(2000);
  Serial.println("GravitoneButtonInterface Test");


  driver.begin(buttonEvent);
}

void buttonEvent(GravitoneButtonEvent event) {
  //Serial.println("got event");
  newEvent = event.id;
}

void loop() {
  // put your main code here, to run repeatedly:
  if( newEvent >= 0 ){
    Serial.print("got new event: ");
    Serial.println(newEvent);
    newEvent = -1;
    
  }

}
