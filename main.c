/**
 * Driver library for HD44780-compatible LCD displays.
 * https://github.com/armandas/Yet-Another-LCD-Library
 *
 * Author: Armandas Jarusauskas (http://projects.armandas.lt)
 *
 * Contributors:
 *    Sylvain Prat (https://github.com/sprat)
 *
 * This work is licensed under a
 * Creative Commons Attribution 3.0 Unported License.
 * http://creativecommons.org/licenses/by/3.0/
 *
 */

#include <p18cxxx.h>
#include <stdio.h>
#include <delays.h>
#include "HD44780.h"

#define MENU_LENGTH     5

#pragma config WDT = OFF
#pragma config PLLDIV = 4
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 2
#pragma config FOSC = HSPLL_HS
#pragma config BOR = OFF
#pragma config PBADEN = OFF
#pragma config PWRT = ON
#pragma config MCLRE = ON
#pragma config XINST = OFF

#pragma code

/******************************************************************************/

unsigned char menu(void);
const rom char * menu_items[MENU_LENGTH] = {
/*  v----LENGTH-----v  */
    "Start Process  ",
    "Info           ",
    "Options        ",
    "About          ",
    "RTFM           "
};

void main(void)
{
    char buffer[LCD_CHARACTERS];

    char item;

    unsigned char degree[8] = {
        0b01100,
    	0b10010,
    	0b10010,
    	0b01100,
    	0b00000,
    	0b00000,
    	0b00000,
    	0b00000
    };

    unsigned char arrow[8] = {
        0b00000,
    	0b01000,
    	0b01100,
    	0b01110,
    	0b01100,
    	0b01000,
    	0b00000,
    	0b00000
    };

    // set RE2 as digital I/O
    ADCON1 = 0x08;

    // set ports as outputs
    TRISC = 0x00;
    LCD_DATA_DDR = 0x00;
    // tactile switch
    TRISE = 0x04;

    lcd_initialize();

    lcd_add_character(1, degree);
    lcd_add_character(2, arrow);

    while (1) {
        item = menu();
        sprintf(buffer, "Selected item: %d", item);
        lcd_goto(2, 1);
        lcd_write(buffer);

        while (PORTEbits.RE2 == 1);
        Delay10KTCYx(255);
    }
}

unsigned char menu(void)
{
    static unsigned char position = 0;
    unsigned char item = 0;

    unsigned char i;

    if (position == MENU_LENGTH)
        // reset position if the end of menu is reached
        position = 0;

    for (i = 0; i < LCD_LINES; i++) {
        // set the line for item display
        lcd_goto(i + 1, 1);

        // enable continuous rotation of items
        // i.e. item 1 follows the last item in the list
        item = (position + i > MENU_LENGTH - 1) ? 0 : position + i;

        if (i == 0)
            // add an arrow to indicate current menu item
            lcd_write_pgm("\002");
        else
            // add a space to keep items aligned
            lcd_write_pgm(" ");

        // display menu item
        lcd_write_pgm(menu_items[item]);
    }
    // return current position and increment counter for the next call
    return position++;
}
