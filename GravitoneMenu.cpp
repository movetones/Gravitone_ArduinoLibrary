#include "GravitoneMenu.h"

GravitoneMenu::GravitoneMenu(struct menu_entry_type *entrys)
{
    menu_entrys = entrys;
}

void GravitoneMenu::begin()
{
    Serial.println("GravitoneMenu::begin()");
    
    current_state.menu_start = ICON_BGAP;
    current_state.frame_position = ICON_BGAP;
    current_state.position = 0;

    destination_state.menu_start = ICON_BGAP;
    destination_state.frame_position = ICON_BGAP;
    destination_state.position = 0;
}

void GravitoneMenu::draw()
{
    int16_t x;
    uint8_t i;
    x = current_state.menu_start;
    i = 0;
    
    Serial.println("menu draw()");
    
    u8g2.clearBuffer();
    
    current_state.frame_position = destination_state.frame_position;
    current_state.menu_start = destination_state.menu_start;
    
    
    while( menu_entrys[i].icon > 0 )  
    {
        if ( x >= -ICON_WIDTH && x < u8g2.getDisplayWidth() )
        {
            u8g2.setFont(menu_entrys[i].font);
            u8g2.drawGlyph(x, ICON_Y, menu_entrys[i].icon );
        }
        i++;
        x += ICON_WIDTH + ICON_GAP;
    }
    //u8g2.drawLine(state.frame_position-1, ICON_Y-ICON_HEIGHT-1, ICON_WIDTH+2, ICON_WIDTH+2);
    //u8g2.drawFrame(state.frame_position-1, ICON_Y-ICON_HEIGHT-1, ICON_WIDTH+2, ICON_WIDTH+2);
    u8g2.drawFrame(current_state.frame_position-2, ICON_Y-ICON_HEIGHT-2, ICON_WIDTH+4, ICON_WIDTH+4);
    u8g2.drawFrame(current_state.frame_position-3, ICON_Y-ICON_HEIGHT-3, ICON_WIDTH+6, ICON_WIDTH+6);
    
    u8g2.setFont(u8g2_font_helvB10_tr);  
    u8g2.setCursor((u8g2.getDisplayWidth()-u8g2.getStrWidth(menu_entrys[destination_state.position].name))/2,u8g2.getDisplayHeight()-5);
    u8g2.print(menu_entrys[destination_state.position].name);    
    u8g2.sendBuffer();
}

void GravitoneMenu::left()
{
    if ( destination_state.position > 0 )
    {
        if ( (int16_t)destination_state.frame_position >= (int16_t)ICON_BGAP+(int16_t)ICON_WIDTH+ (int16_t)ICON_GAP )
        {
            destination_state.position--;
            destination_state.frame_position -= ICON_WIDTH + (int16_t)ICON_GAP;
        }    
        else
        {
            destination_state.position--; 
            destination_state.frame_position = ICON_BGAP;
            destination_state.menu_start = destination_state.frame_position - destination_state.position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
        }
    }
}

void GravitoneMenu::right()
{
    if ( menu_entrys[destination_state.position+1].font != NULL )
    {
        if ( (int16_t)destination_state.frame_position+ 2*(int16_t)ICON_WIDTH + (int16_t)ICON_BGAP < (int16_t)u8g2.getDisplayWidth() )
        {
            destination_state.position++;
            destination_state.frame_position += ICON_WIDTH + (int16_t)ICON_GAP;
        }
        else
        {
            destination_state.position++;      
            destination_state.frame_position = (int16_t)u8g2.getDisplayWidth() - (int16_t)ICON_WIDTH - (int16_t)ICON_BGAP;
            destination_state.menu_start = destination_state.frame_position - destination_state.position*((int16_t)ICON_WIDTH + (int16_t)ICON_GAP);
        }
    }
}
