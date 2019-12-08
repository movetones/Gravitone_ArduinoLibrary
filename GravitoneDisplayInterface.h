#ifndef GRAVITONE_DISPLAY_INTERFACE
#define GRAVITONE_DISPLAY_INTERFACE

#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

class GravitoneDisplayInterface {

public:
    GravitoneDisplayInterface() {}
    
    void begin() {
        Serial.println("GravitoneDisplayInterface::begin()");
        
        pinMode(OLED_CONTROL, OUTPUT);
        digitalWrite(OLED_CONTROL, HIGH);
        
        u8g2.begin();
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.setCursor(10, 10);
        u8g2.print("Hello World!");    
        u8g2.sendBuffer();
    }

};

#endif
