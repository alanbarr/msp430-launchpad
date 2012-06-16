/* Author: Alan Barr
** Created: January 2012
*/

#ifndef LCD_H_
#define LCD_H_

#include "msp430.h"
#include "ST7066_HD44780.h"     /* LCD Header */
#include "time430.h"

/* LCD_READING_FUNCTIONALITY
 * If not defined in the makefile or elsewhere, set here. 
 * 0 Removes the reading functions.
 * 1 Adds in the reading functions. */
#ifndef LCD_READING_FUNCTIONALITY
#define LCD_READING_FUNCTIONALITY           0      
#endif

/*Below are the defines that can be changed*/

#define LCD_DATA_PINS_PORT_DIR              P2DIR
#define LCD_DATA_PINS_PORT_OUT              P2OUT
#define LCD_DATA_PINS_PORT_IN               P2IN
#define LCD_COMMAND_PINS_PORT_DIR           P1DIR
#define LCD_COMMAND_PINS_PORT_OUT           P1OUT

#define LCD_RS                              0x10    /*.4*/
#define LCD_RW                              0x20    /*.5*/
#define LCD_E                               0x80    /*.7*/      
#define LCD_DB4                             0x01    /*.0*/    
#define LCD_DB5                             0x02    /*.1*/    
#define LCD_DB6                             0x04    /*.2*/ 
#define LCD_DB7                             0x08    /*.7*/

/****************************You don't need to change below here****************************/

#include "time430.h"

void lcdInit(void);
void lcdWriteInstruction(const char data);
void lcdWriteDataToRam(const char data);
void lcdPrint(const char const * string);
/* The below require LCD_READING_FUNCTIONALITY to be true*/
char lcdReadBusyFlagAndAddress(void);
char lcdReadDataFromRam(void);

#define LCD_STARTUP_WAIT()                  LCD_HALF_STARTUP_WAIT();

/*Will need broken into smaller Delays for higher frequencies - increases bin size*/
#define LCD_HALF_STARTUP_WAIT()             TIME430_DELAY_MS(100UL); TIME430_DELAY_MS(100UL);   

#define LCD_BUSY_WAIT()                     TIME430_DELAY_MS(1UL)
#define LCD_E_WAIT()                        TIME430_DELAY_US(1UL)
#define LCD_DATA_WAIT()                     TIME430_DELAY_US(200UL)
#define LCD_COMMAND_WAIT()                  TIME430_DELAY_MS(5UL)

#define LCD_COMMAND_PINS                    (LCD_RS | LCD_RW | LCD_E)
#define LCD_DATA_PINS                       (LCD_DB4 | LCD_DB5 | LCD_DB6 | LCD_DB7)
#define LCD_ALL_PINS                        (LCD_COMMAND_PINS | LCD_DATA_PINS)


/*Sets data and command pins to output*/
#define LCD_SET_PINS_OUTPUT()               (LCD_DATA_PINS_PORT_DIR    |= LCD_DATA_PINS); \
                                            (LCD_COMMAND_PINS_PORT_DIR |= LCD_COMMAND_PINS);

/*Sets data pins to input, command pins to output*/
#define LCD_SET_PINS_INPUT()                (LCD_DATA_PINS_PORT_DIR    &= ~LCD_DATA_PINS); \
                                            (LCD_COMMAND_PINS_PORT_DIR |=  LCD_COMMAND_PINS);

#define LCD_SET_E_LOW()                     (LCD_COMMAND_PINS_PORT_OUT &= ~LCD_E);
#define LCD_SET_E_HIGH()                    (LCD_COMMAND_PINS_PORT_OUT |=  LCD_E);

#define LCD_SET_PINS_LOW()                  (LCD_DATA_PINS_PORT_OUT    &= ~LCD_DATA_PINS);\
                                            (LCD_COMMAND_PINS_PORT_OUT &= ~LCD_COMMAND_PINS);

/*Set command pins to correct state*/
#define LCD_SET_PINS_INSTRUCTION_WRITE()    LCD_COMMAND_PINS_PORT_OUT &= ~(LCD_RS | LCD_RW);

#if LCD_READING_FUNCTIONALITY
#define LCD_SET_PINS_INSTRUCTION_READ()     LCD_COMMAND_PINS_PORT_OUT &= ~LCD_RS; \
                                            LCD_COMMAND_PINS_PORT_OUT |=  LCD_RW;

#define LCD_SET_PINS_DATA_READ()            LCD_COMMAND_PINS_PORT_OUT |= (LCD_RS | LCD_RW);
#endif /*LCD_READING_FUNCTIONALITY*/

#define LCD_SET_PINS_DATA_WRITE()           LCD_COMMAND_PINS_PORT_OUT |= (LCD_RS); \
                                            LCD_COMMAND_PINS_PORT_OUT &= ~LCD_RW;

/*Macros for commands. The permanent bits for each command are fixed.*/
#define LCD_CLEAR_DISPLAY()         lcdWriteInstruction(ST7066_CLEAR_DISPLAY_BITS       )
#define LCD_RETURN_HOME()           lcdWriteInstruction(ST7066_RETURN_HOME_BITS         )
#define LCD_ENTRY_MODE_SET(d)       lcdWriteInstruction(ST7066_ENTRY_MODE_BITS     | (d))
#define LCD_DISPLAY_ON_OFF(d)       lcdWriteInstruction(ST7066_DISPLAY_ON_OFF_BITS | (d))
#define LCD_CUR_DISP_SHIFT(d)       lcdWriteInstruction(ST7066_CUR_DISP_SHIFT_BITS | (d))
#define LCD_FUNCTION_SET(d)         lcdWriteInstruction(ST7066_FUNCTION_SET_BITS   | (d))
#define LCD_CGRAM_ADDRESS_SET(d)    lcdWriteInstruction(ST7066_CGRAM_BITS          | (d))
#define LCD_DDRAM_ADDRESS_SET(d)    lcdWriteInstruction(ST7066_DDRAM_BITS          | (d))

#endif /*LCD_H_*/

