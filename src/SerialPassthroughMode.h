#ifndef REPROGRAM_WIFI_MODE
#define REPROGRAM_WIFI_MODE

#include "Gravitone.h"

class SerialPassthroughMode : public GravitoneMode {
public:

  SerialPassthroughMode() {};
  ~SerialPassthroughMode() {};
  
  void start(gtdisplay_t & display) {
    display.setTextSize(0);
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 0);
    display.print("Serial1 pipe");
    display.setCursor(0,20);
    display.print("RST HIGH");
    display.setCursor(0,10);
    display.print("GPIO 0 HIGH");
  };
  
  
  void button4(butevent_t event, gtdisplay_t & display) {
    if( event == BUTTON_PRESSED ){
      pinMode(ESP_RST, OUTPUT);
      digitalWrite(ESP_RST, LOW);
      display.setCursor(0,20);
      display.print("RST LOW ");
    } else if( event == BUTTON_RELEASED ){
      pinMode(ESP_RST, INPUT);
      display.setCursor(0,20);
      display.print("RST HIGH");
    }
  };
  
  void button5(butevent_t event, gtdisplay_t & display) {
    if( event == BUTTON_PRESSED ){
      pinMode(ESP_GPIO0, OUTPUT);
      digitalWrite(ESP_GPIO0, LOW);
      display.setCursor(0,10);
      display.print("GPIO 0 LOW ");
    } else if( event == BUTTON_RELEASED ){
      pinMode(ESP_GPIO0, INPUT);
      display.setCursor(0,10);
      display.print("GPIO 0 HIGH");
    }
  };
};


#endif
