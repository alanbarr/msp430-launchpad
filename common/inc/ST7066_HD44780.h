/* Author: Alan Barr
** Created: Jan 2012
** ST7066/HD44780
** http://www.sparkfun.com/products/9053
*/
#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#include "MCP23017.h.h"         /* i2c Expander Header */

#define LCD_BITEXPANDER_ADDR    0x20
/*Alan TODO current assumption using 4 bit mode - slower but tidier*/
//#define USING_8_BIT_MODE    /*Remove if using 4 bit mode*/
#define USING_4_BIT_MODE

/* LCD Pins */
/* Data pins are expected to be on respective i2c expander pin, e.g. DB0 is on
** i2c expander pin 0 on either port A or port B. RS, RW and E can be on any pin
** (but all on the same port).
*/

/*Specify the i2c Expander address of the GPIO output register containing RS,RW and E*/
#define I2C_PORT_ADDRESS_OP_PINS    GPIOA
#define RS      0x01 
#define RW      0x02
#define E       0x04

/*Specify the i2c Expander address of the GPIO output register containing DB0-DB7*/
#define I2C_PORT_ADDRESS_DATA_PINS  GPIOA

#define DB4     0x10
#define DB5     0x20
#define DB6     0x40
#define DB7     0x80

#ifdef USING_8_BIT_MODE
#define DB0     0x01
#define DB1     0x02
#define DB2     0x04
#define DB3     0x80
#endif /*USING_8_BIT_MODE*/

/* Clear Display Settings */
#define CLEAR_DISPLAY_BITS  0x00

/* Return Home Settings */
#define RETURN_HOME_BITS    0x02

/*Entry Mode Settings */
#define ENTRY_MODE_BITS     0x04
#define MOVE_CURSOR_RIGHT   0x02
#define MOVE_CURSOR_LEFT    0x00

#define SHIFT_DISPLAY       0x01
#define SHIFT_LEFT          0x02
#define SHIFT_RIGHT         0x00

/* Display On Off Settings */
#define DISPLAY_ON_OFF_BITS 0x08
#define DISPLAY_ON          0x04
#define DISPLAY_OFF         0x00
#define CURSOR_ON           0x02
#define CURSOR_OFF          0x00
#define CURSOR_BLINK_ON     0x01
#define CURSOR_BLINK_OFF    0x00

/*Set CGRAM Address Settings */
#define CGRAM_BITS          0x40

/*Set DDRAM Address Settings */
#define DDRAM_BITS          0x80

/* Function Set Settings */
#define FUNCTION_SET_BITS   0x20
#define BIT_MODE_4          0x00
#define BIT_MODE_8          0x10
#define LINE_MODE_1         0x00
#define LINE_MODE_2         0x08
#define FONT_MODE_5x8       0x00
#define FONT_MODE_5x11      0x40


/* Read Busy Flag and Address Settings */
/*Alan TODO*/

/* Write Data Settings */
/*Alan TODO*/

/* Read Data Settings */
/*Alan TODO*/

#define LINE0_POS0  1
#define LINE0_POS1  2
#define LINE0_POS2  3
#define LINE0_POS3  4
#define LINE0_POS4  5
#define LINE0_POS5  6
#define LINE0_POS7  7
#define LINE0_POS8  8
#define LINE0_POS9  9
#define LINE0_POS10 10
#define LINE0_POS11 11
#define LINE0_POS12 12
#define LINE0_POS13 13
#define LINE0_POS14 14
#define LINE0_POS15 15
#define LINE0_POS16 16
#define LINE0_POS17 17
#define LINE0_POS18 18
#define LINE0_POS19 19

#define LINE1_POS0  64
#define LINE1_POS1  65
#define LINE1_POS2  66
#define LINE1_POS3  67
#define LINE1_POS4  68
#define LINE1_POS5  69
#define LINE1_POS7  70
#define LINE1_POS8  71
#define LINE1_POS9  72
#define LINE1_POS10 73
#define LINE1_POS11 74
#define LINE1_POS12 75  
#define LINE1_POS13 76  
#define LINE1_POS14 77  
#define LINE1_POS15 78  
#define LINE1_POS16 79  
#define LINE1_POS17 80  
#define LINE1_POS18 81  
#define LINE1_POS19 82  



#endif /*I2C_LCD_H_*/
