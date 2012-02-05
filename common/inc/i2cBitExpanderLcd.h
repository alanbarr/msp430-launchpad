/* Author: Alan Barr
** Created: January 2012
*/

#ifndef I2C_BIT_EXP_LCD_H_
#define I2C_BIT_EXP_LCD_H_

#define TIME430_CLOCK_FREQ 1

#include "i2c.h"                
#include "MCP23017.h"           /* i2c Expander Header */
#include "ST7066_HD44780.h"     /* LCD Header */
#include "time430.h"

void i2cLcdInit(void);
void i2cLcdInstructionSendCommand(const char data);
char i2cLcdReadBusyFlagAndAddress(void);
void i2cLcdWriteDataToRam(const char data);
char i2cLcdReadDataFromRam(void);
void i2cLcdChangeExpanderPinsInput(void);
void i2cLcdChangeExpanderPinsOutput(void);
void i2cLcdPrint(const char const * string);

#define LCD_BITEXPANDER_ADDR                MCP23017_ADDRESS
#define LCD_PORT_ADDRESS_DIRECTION          MCP23017_IODIRA
#define LCD_PORT_ADDRESS_OUTPUT             MCP23017_GPIOA

#define LCD_STARTUP_WAIT()                  TIME430_DELAY_MS(100U);\
                                            TIME430_DELAY_MS(100U);

#define LCD_BUSY_WAIT()                     TIME430_DELAY_MS(10UL)
#define LCD_E_WAIT()                        TIME430_DELAY_US(1UL)
#define LCD_DATA_WAIT()                     TIME430_DELAY_US(250UL)
#define LCD_COMMAND_WAIT()                  TIME430_DELAY_MS(5UL)

#define LCD_RS                              0x01        
#define LCD_RW                              0x02        
#define LCD_E                               0x04        
#define LCD_DB4                             0x10        
#define LCD_DB5                             0x20        
#define LCD_DB6                             0x40        
#define LCD_DB7                             0x80        

#define LCD_PINS_OUTPUT                     (0xFF & ~(LCD_RS | LCD_RW | LCD_E |\
                                             LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7))

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


#endif /*I2C_BIT_EXP_LCD_H_*/
