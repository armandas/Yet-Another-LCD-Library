/*********************************** CONFIG ***********************************/

// 4-bit mode
#define DL (0 << 4)
// 2-line display
#define N  (1 << 3)
// 5x8 character font
#define F  (0 << 2)

// pins
#define DATA LATD
#define RS LATCbits.LATC0
#define RW LATCbits.LATC1
#define EN LATCbits.LATC2

/******************************************************************************/

// commands
#define CLEAR_DISPLAY   0x01
#define RETURN_HOME     0x02
#define DISPLAY_CONTROL 0x08
#define FUNCTION_SET    0x20
#define SET_DDRAM_ADDR  0x80

// flags
#define DISPLAY_ON  0x04
#define CURSOR_ON   0x02
#define BLINKING_ON 0x01

// function prototypes
void _send_nibble(unsigned char data);
void _send_byte(unsigned char data);
void _display_control_set_on(unsigned char flags);
void _display_control_set_off(unsigned char flags);

void lcd_initialize(void);
void lcd_command(unsigned char command);
void lcd_data(unsigned char data);
void lcd_write(const rom char * str);
void lcd_goto(unsigned char row, unsigned char col);

// inline functions
#define lcd_clear()         lcd_command(CLEAR_DISPLAY)
#define lcd_return_home()   lcd_command(RETURN_HOME)

#define lcd_display_on()    _display_control_set_on(DISPLAY_ON)
#define lcd_display_off()   _display_control_set_off(DISPLAY_ON)
#define lcd_cursor_on()     _display_control_set_on(CURSOR_ON)
#define lcd_cursor_off()    _display_control_set_off(CURSOR_ON)
#define lcd_blinking_on()   _display_control_set_on(BLINKING_ON)
#define lcd_blinking_off()  _display_control_set_off(BLINKING_ON)
