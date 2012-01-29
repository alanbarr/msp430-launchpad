/* Author: Alan Barr
** Created: January 2012

This header file interacts with the LCD via an i2c bit expander. */

#ifndef I2C_BIT_EXP_LCD_H_
#define I2C_BIT_EXP_LCD_H_

#define TIME430_CLOCK_FREQ 1

#include "i2c.h"                
#include "MCP23017.h"           /* i2c Expander Header */
#include "ST7066_HD44780.h"     /* LCD Header */
#include "usefulFunctions.h"
#include "time430.h"

void i2cLcdInit(void);
void i2cLcdInstructionSendCommand(const char data);
char i2cLcdReadBusyFlagAndAddress(void);
void i2cLcdWriteDataToRam(const char data);
char i2cLcdReadDataFromRam(void);
void i2cLcdChangeExpanderPinsInput(void);
void i2cLcdChangeExpanderPinsOutput(void);
void i2cLcdPrint(const char const * string);
/* Various delays for different scenario's during LCD operation. I tried to 
** balance out reducing delay time but also keeping the number of different 
** delays at a minimum. Some of the delays are probably a little generous,
** so if you feel it is too slow you might be able to cut a few down. 
** Especially since I am not factoring in i2c times to bit expander.
*/
#define LCD_STARTUP_WAIT()                  TIME430_DELAY_MS(10U)
#define LCD_BUSY_WAIT()                     TIME430_DELAY_MS(10U)
#define LCD_E_HIGH_WAIT()                   TIME430_DELAY_US(1U)
#define LCD_DATA_WAIT()                     TIME430_DELAY_US(250U)
#define LCD_COMMAND_WAIT()                  TIME430_DELAY_US(5U)

/* LCD Pins */
/* Data pins are expected to be on respective i2c expander pin, e.g. DB0 is on
** i2c expander pin 0 on either port A or port B. RS, RW and E can be on any pin
** (but all on the same port).
*/
#define LCD_BITEXPANDER_ADDR                MCP23017_ADDRESS

/*Alan TODO current assumption using 4 bit mode - slower but frees more pins*/
#define LCD_USING_4_BIT_MODE

#ifdef LCD_USING_4_BIT_MODE
    /*Specify the i2c Expander address of the GPIO output register 
    **containing RS,RW and E*/
    #define LCD_PORT_ADDRESS_OP_PINS        GPIOA
    /*Specify the i2c Expander address of the GPIO output register
    **containing DB0-DB7*/
    #define LCD_PORT_ADDRESS_DATA_PINS      GPIOA

    #define LCD_RS                          0x01        
    #define LCD_RW                          0x02        
    #define LCD_E                           0x04        
    #define LCD_DB4                         0x10        
    #define LCD_DB5                         0x20        
    #define LCD_DB6                         0x40        
    #define LCD_DB7                         0x80        
#endif /*LCD_USING_4_BIT_MODE*/


#ifdef LCD_USING_8_BIT_MODE
    /*Specify the i2c Expander address of the GPIO output register 
    **containing RS,RW and E*/
    #define LCD_PORT_ADDRESS_OP_PINS        GPIOA
    /*Specify the i2c Expander address of the GPIO output register 
    **containing DB0-DB7*/
    #define LCD_PORT_ADDRESS_DATA_PINS      GPIOB

    #define LCD_RS                          0x01        
    #define LCD_RW                          0x02        
    #define LCD_E                           0x04        
    #define LCD_DB0                         0x01        
    #define LCD_DB1                         0x02        
    #define LCD_DB2                         0x04        
    #define LCD_DB3                         0x08        
    #define LCD_DB4                         0x10        
    #define LCD_DB5                         0x20        
    #define LCD_DB6                         0x40        
    #define LCD_DB7                         0x80        
#endif  /*LCD_USING_8_BIT_MODE*/


#if LCD_PORT_ADDRESS_DATA_PINS == LCD_PORT_ADDRESS_OP_PINS
    #define LCD_PINS_ADDRESS LCD_PORT_ADDRESS_DATA_PINS
#endif


/* Bit Configs */
/* LCD Pins Low, set to zero for bit exp. All else high */
#define LCD_PINS_OUTPUT                     (0xFF & ~(LCD_RS | LCD_RW | LCD_E |\
                                            LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7))
/* LCD data pins changed to input */
#define LCD_PINS_INPUT                      (0xFF & ~(LCD_RS | LCD_RW | LCD_E))

/*Macros for commands. The permanent bits for each command are fixed.*/
#define LCD_CLEAR_DISPLAY()         i2cLcdInstructionSendCommand(ST7066_CLEAR_DISPLAY_BITS  )
#define LCD_RETURN_HOME()           i2cLcdInstructionSendCommand(ST7066_RETURN_HOME_BITS    )
#define LCD_ENTRY_MODE_SET(d)       i2cLcdInstructionSendCommand(ST7066_ENTRY_MODE_BITS     | (d))
#define LCD_DISPLAY_ON_OFF(d)       i2cLcdInstructionSendCommand(ST7066_DISPLAY_ON_OFF_BITS | (d))
#define LCD_CUR_DISP_SHIFT(d)       i2cLcdInstructionSendCommand(ST7066_CUR_DISP_SHIFT_BITS | (d))
#define LCD_FUNCTION_SET(d)         i2cLcdInstructionSendCommand(ST7066_FUNCTION_SET_BITS   | (d))
#define LCD_CGRAM_ADDRESS_SET(d)    i2cLcdInstructionSendCommand(ST7066_CGRAM_BITS          | (d))
#define LCD_DDRAM_ADDRESS_SET(d)    i2cLcdInstructionSendCommand(ST7066_DDRAM_BITS          | (d))


#define LCD_BUSY_FLAG           0x80
#define LCD_INSTRUCTION_WRITE
#define LCD_INSTRUCTION_READ
#define LCD_DATA_WRITE
#define LCD_DATA_READ
#endif /*I2C_BIT_EXP_LCD_H_*/
