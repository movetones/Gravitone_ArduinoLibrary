#ifndef REPROGRAM_WIFI_MODE
#define REPROGRAM_WIFI_MODE

#include "Gravitone.h"

class SerialPassthroughMode : public GravitoneMode {
public:

  SerialPassthroughMode() {};
  ~SerialPassthroughMode() {};
  
  void start() {
    hardware->display.setTextSize(0);
    hardware->display.setTextColor(WHITE, BLACK);
    hardware->display.setCursor(0, 0);
    hardware->display.print("Serial1 pipe");
    hardware->display.setCursor(0,20);
    hardware->display.print("RST HIGH");
    hardware->display.setCursor(0,10);
    hardware->display.print("GPIO 0 HIGH");
  };
  
  
  void button4(butevent_t event) {
    if( event == BUTTON_PRESSED ){
      pinMode(ESP_RST, OUTPUT);
      digitalWrite(ESP_RST, LOW);
      hardware->display.setCursor(0,20);
      hardware->display.print("RST LOW ");
    } else if( event == BUTTON_RELEASED ){
      pinMode(ESP_RST, INPUT);
      hardware->display.setCursor(0,20);
      hardware->display.print("RST HIGH");
    }
  };
  
  void button5(butevent_t event) {
    if( event == BUTTON_PRESSED ){
      pinMode(ESP_GPIO0, OUTPUT);
      digitalWrite(ESP_GPIO0, LOW);
      hardware->display.setCursor(0,10);
      hardware->display.print("GPIO 0 LOW ");
    } else if( event == BUTTON_RELEASED ){
      pinMode(ESP_GPIO0, INPUT);
      hardware->display.setCursor(0,10);
      hardware->display.print("GPIO 0 HIGH");
    }
  };
};


#endif
