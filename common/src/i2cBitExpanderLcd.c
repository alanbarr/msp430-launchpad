/* Author: Alan Barr 
 * Created: January 2012 */
/* This code is used to control a ST7066/HD44780 lcd via a MCP23017
** bit expander. This is the display used: http://www.sparkfun.com/products/9053
*/

#include "i2cBitExpanderLcd.h"

/* Provides initialisation for both the bit expander and the LCD.
** Bit Expander pointer address incrementing disabled and LCD
**  connected pins are changed to low outputs.
** LCD initalisation routine is conducted, after which LCD is
**  put into 4 bit mode, display cleared and returned to, 
**  home. The cursor is set to move right and the display 
**  is finally turned on.
*/
void i2cLcdInit(void)
{   
    char transmitBuffer[2] = {MCP23017_IOCON, MCP23017_SEQOP};

    /* Disable MCP23017 pointer incrementing */ 
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);

    /* Setup LCD pins as output - leave other pins as input */
    i2cLcdChangeExpanderPinsOutput();

    /* Setup LCD pins as low */
    transmitBuffer[0] = LCD_PORT_ADDRESS_OUTPUT;
    transmitBuffer[1] = 0x00;
    i2cTransmit(transmitBuffer,2);
    
    /* LCD Startup */
    LCD_STARTUP_WAIT();
    
    /*Prepare pins*/
    transmitBuffer[1] = ST7066_START_UP_COMMAND;
    i2cTransmit(transmitBuffer,2);

    /*First clock*/
    transmitBuffer[1] |= ST7066_E;
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] = ST7066_START_UP_COMMAND;
    i2cTransmit(transmitBuffer,2);
    TIME430_DELAY_MS(5);
    /*Second clock*/
    transmitBuffer[1] |= ST7066_E;
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] = ST7066_START_UP_COMMAND;
    i2cTransmit(transmitBuffer,2);
    TIME430_DELAY_US(200);
    /*Third clock*/
    transmitBuffer[1] |= ST7066_E;
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] = 0x00;
    i2cTransmit(transmitBuffer,2);
    TIME430_DELAY_US(200);
 
    /*Set display to 4 bit mode*/
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    transmitBuffer[1] = 0x20;
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    i2cTransmit(transmitBuffer,2);
    
    TIME430_DELAY_MS(1);
    transmitBuffer[1] &= ~ST7066_E;
    i2cTransmit(transmitBuffer,2);
    TIME430_DELAY_MS(1);
    transmitBuffer[1] &= 0x00;
    i2cTransmit(transmitBuffer,2);

    TIME430_DELAY_MS(100UL);
    
    /*Set up LCD now that it is in 4 bit mode*/
    LCD_FUNCTION_SET(ST7066_BIT_MODE_4 | ST7066_LINE_MODE_2);
    LCD_CLEAR_DISPLAY();
    LCD_RETURN_HOME();
    LCD_ENTRY_MODE_SET(ST7066_MOVE_CURSOR_RIGHT); 
    LCD_DISPLAY_ON_OFF(ST7066_DISPLAY_ON);
}

/* Writes Instructions to the LCD i.e. RS and RW are low. 
** The argument data is the data bits on the lcd. */
void i2cLcdInstructionSendCommand(const char data)
{
    char transmitBuffer[2] = {LCD_PORT_ADDRESS_OUTPUT, (data & 0xF0) | 
                              ST7066_INSTRUCTION_WRITE };
    
    /*Clocking Upper Nibble*/
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] &= ~ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] = 0x00;
    i2cTransmit(transmitBuffer,2);
    /*Clocked Upper Nibble */

    LCD_COMMAND_WAIT();

    /*Clocking Lower Nibble */
    transmitBuffer[1] = (data<<4 & 0xF0) | ST7066_INSTRUCTION_WRITE;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] &= ~ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] = 0x00;
    i2cTransmit(transmitBuffer,2);
    /*Lower Nibble Clocked*/

    LCD_COMMAND_WAIT();
}


/* Reads Instructions from the LCD i.e. RS Low and RW high. Returns char 
** containing address bits and busy flag (the highest order bit. */
char i2cLcdReadBusyFlagAndAddress(void)
{
    char readData = 0;
    char returnData = 0;
    char transmitBuffer[2] = {LCD_PORT_ADDRESS_OUTPUT, ST7066_INSTRUCTION_READ};
    
    i2cLcdChangeExpanderPinsInput();
    
    /*Reading Upper Nibble*/
    transmitBuffer[1] = ST7066_INSTRUCTION_READ;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    
    i2cSetupRx(LCD_BITEXPANDER_ADDR);
    i2cReceive(&readData,1);
    returnData = (readData & 0xF0);

    transmitBuffer[1] &= ~ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    /*Upper Nibble Read */
    
    LCD_COMMAND_WAIT();

    /*Reading Lower Nibble */
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    
    i2cSetupRx(LCD_BITEXPANDER_ADDR);
    i2cReceive(&readData,1);
    returnData |= (readData >>4);
    
    transmitBuffer[1] &= ~ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    /*Lower Nibble Read*/

    LCD_COMMAND_WAIT();

    i2cLcdChangeExpanderPinsOutput();
    
    return returnData;
}


/* Writes Instructions to the LCD i.e. RS high RW low. The argument data is the desired state
** of LCD pins DB0 - DB7*/
void i2cLcdWriteDataToRam(const char data)
{
    char transmitBuffer[2] = { LCD_PORT_ADDRESS_OUTPUT, (data & 0xF0) | ST7066_DATA_WRITE };

    /*Clocking Upper Nibble*/
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] &= ~ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    /*Clocked Upper Nibble */

    LCD_DATA_WAIT();

    /*Clocking Lower Nibble */
    transmitBuffer[1] = (data<<4 & 0xF0) | ST7066_DATA_WRITE;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    LCD_E_WAIT();
    transmitBuffer[1] &= ~ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    /*Lower Nibble Clocked*/

    LCD_DATA_WAIT();

}


/* Reads data from the LCD i.e. RS and RW high. Returns this is
** char return.*/
char i2cLcdReadDataFromRam(void)
{
    char transmitBuffer[2] = {LCD_PORT_ADDRESS_OUTPUT, ST7066_DATA_READ };
    char readData = 0;
    char returnData = 0;

    i2cLcdChangeExpanderPinsInput();

    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);

    /*Clocking Upper Nibble*/
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    LCD_E_WAIT();
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    i2cSetupRx(LCD_BITEXPANDER_ADDR);
    i2cReceive(&readData,1);
    returnData = readData << 4;

    transmitBuffer[1] &= ~ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    /*Clocked Upper Nibble */

    LCD_DATA_WAIT();

    /*Clocking Lower Nibble */
    transmitBuffer[1] = ST7066_DATA_READ;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[1] |= ST7066_E;
    LCD_E_WAIT();
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    
    i2cSetupRx(LCD_BITEXPANDER_ADDR);
    i2cReceive(&readData,1);
    returnData |= (readData & 0x0F);

    transmitBuffer[1] &= ~ST7066_E;
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    /*Lower Nibble Clocked*/

    LCD_DATA_WAIT(); 
    i2cLcdChangeExpanderPinsOutput();
    
    return readData;
}


/* Sets the LCD data Bit Expander Bits to input.
** Sets pins to low.*/
void i2cLcdChangeExpanderPinsInput(void)
{
    char transmitBuffer[2] = { LCD_PORT_ADDRESS_DIRECTION, LCD_PINS_INPUT };
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[0] = LCD_PORT_ADDRESS_OUTPUT;
    transmitBuffer[1] = 0x00;
}


/* Sets the LCD data Bit Expander Bits to output.
** Sets pins to low.*/
void i2cLcdChangeExpanderPinsOutput(void)
{
    char transmitBuffer[2] = { LCD_PORT_ADDRESS_DIRECTION, LCD_PINS_OUTPUT };
    i2cSetupTx(LCD_BITEXPANDER_ADDR);
    i2cTransmit(transmitBuffer,2);
    transmitBuffer[0] = LCD_PORT_ADDRESS_OUTPUT;
    transmitBuffer[1] = 0x00;
}


void i2cLcdPrint(const char const * string)
{
    int index = 0;

    while(string[index] != '\0')
    {
        i2cLcdWriteDataToRam(string[index]);
        index++;
    }

}

