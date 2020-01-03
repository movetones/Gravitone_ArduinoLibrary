#ifndef GRAVITONE_MENU
#define GRAVITONE_MENU

#include <U8g2lib.h>
#include "Gravitone.h"
#include "GravitoneButtonDriver.h"

// menu structs and drawing from u8g2 icon menu examples 
// https://github.com/olikraus/U8g2_Arduino/blob/master/examples/full_buffer/IconMenu/IconMenu.ino

/*
  Icon configuration
  Width and height must match the icon font size
  GAP: Space between the icons
  BGAP: Gap between the display border and the cursor.
*/
#define ICON_WIDTH 16
#define ICON_HEIGHT 16
#define ICON_GAP 6
#define ICON_BGAP 10
#define ICON_Y 25+ ICON_GAP

// Gravitone button driver to get menu keypresses
extern GravitoneButtonDriver driver;

// u8g2 driver
extern U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2;

extern int lastButtonEvent;

struct menu_entry_type
{
  const uint8_t *font;
  uint16_t icon;
  const char *name;
};

struct menu_state
{
  int16_t menu_start;		/* in pixel */
  int16_t frame_position;	/* in pixel */
  uint8_t position;		/* position, array index */
};

class GravitoneMenu {

public:
    GravitoneMenu(struct menu_entry_type *menu_entrys);

    void begin();
    void draw();
    void right();
    void left();

private:
    struct menu_entry_type *menu_entrys;
    struct menu_state current_state;
    struct menu_state destination_state;

};

#endif
