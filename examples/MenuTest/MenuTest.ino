#include <GravitoneMenu.h>

// Gravitone button driver to get menu keypresses
GravitoneButtonDriver driver = GravitoneButtonDriver::driver;

// u8g2 driver
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int lastButtonEvent = -1;

struct menu_entry_type menu_entrys[] =
{
  { u8g2_font_open_iconic_play_2x_t, 80, "Volume"},
  { u8g2_font_open_iconic_play_2x_t, 77, "Scales"},
  { u8g2_font_open_iconic_other_2x_t, 68, "Pads"},
  { NULL, 0, NULL } 
};

GravitoneMenu menu(menu_entrys);

void onButtonPress(GravitoneButtonEvent e)
{
    if( e.value == 0 && e.id <= 2 )
    {
        Serial.println("got menu button event");
    
        if( e.id == 0 )
        {
            menu.left();
        }
        else if( e.id == 2 )
        {
            menu.right();
        }
        Serial.println("drawing...");
        menu.draw();
    }
}

void setup() {


  // put your setup code here, to run once:
  Serial.begin(11500);

    delay(2000);
  Serial.println("GravitoneMenu Test");


// TODO: put this in generic gravitone driver startup
    pinMode(OLED_CONTROL, OUTPUT);
    digitalWrite(OLED_CONTROL, HIGH);

  u8g2.begin();
  driver.begin(onButtonPress);
  
  menu.begin();
  menu.draw();
}

void loop() {
  // put your main code here, to run repeatedly:

}
