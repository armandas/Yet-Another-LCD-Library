#include <delays.h>
#include "HD44780.h"

// global variables
unsigned char display_config[6];

/**
 * Display string stored in RAM
 *
 * Usage:
 *     char message[10] = "Hello";
 *     lcd_write(message);
 */
void lcd_write(char * str) {
    unsigned char i = 0;

    while (str[i] != '\0')
        lcd_data(str[i++]);
}

/**
 * Display string stored in program memory
 *
 * Usage:
 *     lcd_write_pgm("Hello");
 */
void lcd_write_pgm(const rom char * str) {
    unsigned char i = 0;

    while (str[i] != '\0')
        lcd_data(str[i++]);
}

/**
 * Move cursor to a given location
 */
void lcd_goto(unsigned char row, unsigned char col) {
    unsigned char addr;

    switch (row) {
        case 3:
            // fall through
        case 4:
            addr = ((row - 3) * 0x40) + 0x14 + col - 1;
            break;
        default:
            // rows 1 and 2
            addr = ((row - 1) * 0x40) + col - 1;
            break;
    }

    lcd_command(SET_DDRAM_ADDR | addr);
}

/**
 * Add a custom character
 */
void lcd_add_character(unsigned char addr, unsigned char * pattern) {
    unsigned char i;

    lcd_command(SET_CGRAM_ADDR | addr << 3);
    for (i = 0; i < 8; i++)
        lcd_data(pattern[i]);
}

/**
 * Lousy function for automatic LCD initialization
 */
void lcd_initialize(void) {
    unsigned char i;

    // set relevant pins as outputs
    LCD_DATA_DDR = ~DATA_MASK;
    LCD_RS_DDR = 0;
    LCD_RW_DDR = 0;
    LCD_EN_DDR = 0;

    #ifdef LCD_HAS_BACKLIGHT
    LCD_BL_DDR = 0;
    #endif

    // initialize the display_config
    for (i = 0; i < 6; i++) {
        display_config[i] = 0x00;
    }

    LCD_RS = 0;
    LCD_RW = 0;

    // set 4-bit mode
    _send_nibble(0b0010);

    // function set
    lcd_flags_set(FUNCTION_SET, DATA_LENGTH | CHAR_FONT, 0);
    lcd_flags_set(FUNCTION_SET, DISPLAY_LINES, 1);

    #ifdef LCD_HAS_BACKLIGHT
    lcd_backlight_on();
    #endif
    lcd_display_on();
    lcd_cursor_off();
    lcd_blinking_off();

    lcd_flags_set(ENTRY_MODE, CURSOR_INCREMENT, 1);
    lcd_flags_set(ENTRY_MODE, ENABLE_SHIFTING, 0);

    lcd_clear();
    lcd_return_home();
}


/***************************************/
/*        Low Level Functions          */
/***************************************/
void _send_nibble(unsigned char data) {
	data |= data << 4;  		  // copy the data to the upper bits
    LCD_DATA &= ~DATA_MASK;       // clear old data bits
    LCD_DATA |= DATA_MASK & data; // put in new data bits

    // clock
    Delay1KTCYx(1);
    LCD_EN = 1;
    Delay1KTCYx(1);
    LCD_EN = 0;
    Delay1KTCYx(1);
}

void _send_byte(unsigned char data) {
    _send_nibble(data >> 4);
    _send_nibble(data & 0x0F);
}

void lcd_command(unsigned char command) {
    LCD_RS = 0;
    LCD_RW = 0;
    _send_byte(command);
    Delay10KTCYx(1);
}

void lcd_data(unsigned char data) {
    LCD_RS = 1;
    LCD_RW = 0;
    _send_byte(data);
}

void lcd_flags_set(unsigned char instruction,
                   unsigned char flags,
                   unsigned char value)
{
    unsigned char index;

    switch (instruction) {
        case ENTRY_MODE:
            index = 0;
            break;
        case DISPLAY_CONTROL:
            index = 1;
            break;
        case CURSOR_DISPLAY_SHIFT:
            index = 2;
            break;
        case FUNCTION_SET:
            index = 3;
            break;
        case SET_CGRAM_ADDR:
            index = 4;
            break;
        case SET_DDRAM_ADDR:
            index = 5;
            break;
    }

    if (value == 0)
        display_config[index] &= ~flags; // reset flags
    else
        display_config[index] |= flags; // set flags

    lcd_command(instruction | display_config[index]);
}
