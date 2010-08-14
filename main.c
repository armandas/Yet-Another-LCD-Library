#include <p18cxxx.h>
#include "HD44780.h"

#pragma config WDT = OFF
#pragma config PLLDIV = 10
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

void main(void)
{
    unsigned char ghost1[8] = {
        0b01110,
    	0b11111,
    	0b10101,
    	0b11111,
    	0b11111,
    	0b10101,
    	0b00000,
    	0b00000
    };

    unsigned char ghost2[8] = {
        0b00000,
        0b01110,
    	0b11111,
    	0b10101,
    	0b11111,
    	0b11111,
    	0b10101,
    	0b00000
    };

    // set ports as outputs
    TRISC = 0x00;
    TRISD = 0x00;

    lcd_initialize();

    lcd_add_character(0, ghost1);
    lcd_add_character(1, ghost2);

    lcd_goto(1, 5);
    lcd_write("Armandas");

    while (1) {
        lcd_goto(1,1);
        lcd_data(0);
        lcd_goto(1, 16);
        lcd_data(1);
        Delay10KTCYx(255);
        Delay10KTCYx(255);
        lcd_goto(1,1);
        lcd_data(1);
        lcd_goto(1, 16);
        lcd_data(0);
        Delay10KTCYx(255);
        Delay10KTCYx(255);
    }
}
