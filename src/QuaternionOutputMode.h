#ifndef QUATERNION_OUTPUT_MODE_H
#define QUATERNION_OUTPUT_MODE_H

#include <ArduinoJson.h>
#include "GravitoneMode.h"


class QuaternionOutputMode : public GravitoneMode {
public:
  QuaternionOutputMode() : lastWsSend(0), sendTimeout(60), sending(false) {}
  ~QuaternionOutputMode() {};
  
  void start() {
    hardware->display.setTextSize(0);
    hardware->display.setTextColor(WHITE, BLACK);
    hardware->display.setCursor(0, 0);
    hardware->display.print("Quaternion Output");
  }
  
  void stop() {
    hardware->display.clearDisplay();
  }
  
  void onUpdateOrientation() {
    now = millis();
    if( now - lastWsSend > sendTimeout && sending){
      doc["type"] = "data";
      doc["data"]["quat_w"] = hardware->getQ0();
      doc["data"]["quat_x"] = hardware->getQ1();
      doc["data"]["quat_y"] = hardware->getQ2();
      doc["data"]["quat_z"] = hardware->getQ3();
      serializeJson(doc, Serial);
      Serial.print("\r\n");
      lastWsSend = now;
    }
  }
  
  void onUpdateDisplay() {
     hardware->display.setCursor(0,12);
     hardware->display.print(" Sending: ");
     hardware->display.print(sending ? "YES" : "NO ");
  }
  
  void button4(butevent_t event) {
    if( event == BUTTON_PRESSED ){
      sending = true;
    } else if( event == BUTTON_RELEASED ){
      sending = false;
    }
  }
  
  void button9(butevent_t event) {
    if( event == BUTTON_PRESSED ){
      sending = !sending;
    }
  }
  
private:
  bool sending;
  unsigned long lastWsSend, now;
  int sendTimeout;
  StaticJsonDocument<200> doc;

};

#endif
