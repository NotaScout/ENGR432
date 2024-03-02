// PINSSSS

// RW PINS
// GPIOE
#define RS 5

// GPIOE
#define RW 6

// GPIOE
#define EN 3

// // DATA


// GPIOF
#define DB4 8

// GPIOF
#define DB5 7

// GPIOF
#define DB6 9

// GPIOG
#define DB7 1

void LCD_PIN_clear(int PINNO);
void LCD_PIN_set(int PINNO);
void LCD_BF_check(void);
void LCD_PIN_MODE(int PINNO, unsigned int MODE);

void LCD_port_init(void);
void LCD_init(void);

void LCD_sendInstr(unsigned char Instruction);
void LCD_sendData(unsigned char data);
void LCD_clear(void);

void LCD_send_String(char inputstring[]);
void LCD_send_intString(int integer_number);
void LCD_send_floatString(float float_number);

void LCD_place_cursor(unsigned char lineno);//0x80 for 1st line and 0xC0 for 2nd line. Now sends a bit


/* UNUSED //




// write these
 //


// PIN set and clear functions


*/




/*

void LCD_send_character(char input);
void LCD_send_int(int integer);
void LCD_send_float(float floating_point_num);

// Simplification Routines



void full_reset(void);
*/


