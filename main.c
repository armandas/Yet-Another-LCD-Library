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
    // set ports as outputs
    TRISC = 0x00;
    TRISD = 0x00;

    lcd_initialize();

    lcd_goto(1, 5);
    lcd_write("Armandas");
    lcd_goto(2, 1);
    lcd_write("www.armandas.lt");
    
    while (1);  
}
