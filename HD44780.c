#include <delays.h>
#include <string.h>
#include "HD44780.h"

// global variables
unsigned char display_config = 0x00;

void _send_nibble(unsigned char data)
{
    DATA = 0x0f & data;
    Delay1KTCYx(1);
    EN = 1;
    Delay1KTCYx(1);
    EN = 0;
    Delay1KTCYx(1);
}

void _send_byte(unsigned char data)
{
    _send_nibble(data >> 4);
    _send_nibble(data);
}

void lcd_command(unsigned char command)
{
    RS = 0;
    _send_byte(command);
}

void lcd_data(unsigned char data)
{
    RS = 1;
    _send_byte(data);
}

void _display_control_set_on(unsigned char flags)
{
    display_config |= flags;
    lcd_command(DISPLAY_CONTROL | display_config);
}

void _display_control_set_off(unsigned char flags)
{
    display_config &= ~flags;
    lcd_command(DISPLAY_CONTROL | display_config);
}

void lcd_write(const rom char * str)
{
    int i;
    int len = strlenpgm(str);

    for (i = 0; i < len; i++)
        lcd_data(str[i]);
}

void lcd_goto(unsigned char row, unsigned char col)
{
    unsigned char addr = ((row - 1) * 0x40) + col - 1;
    lcd_command(SET_DDRAM_ADDR | addr);
}

void lcd_initialize(void)
{
    RS = 0;
    RW = 0;

    // set 4-bit mode
    _send_nibble(0b0010);

    // function set
    lcd_command(FUNCTION_SET | DL | N | F);

    lcd_display_on();
    lcd_cursor_off();
    lcd_blinking_off();

    // cursor direction and display shift
    //send_byte(0b00000110);

    lcd_clear();
    lcd_return_home();
}
