/* Author: Alan Barr 
 * Created: January 2012 */

/* Provides initialisation for the LCD.
** LCD connected pins are changed to low outputs.
** LCD initalisation routine is conducted, after which LCD is
**  put into 4 bit mode, display cleared and returned to, 
**  home. The cursor is set to move right and the display 
**  is finally turned on.
*/

#include "lcd_st7066.h"

static void lcdSetUpperNibble(const char data);
static void lcdSetLowerNibble(const char data);
static void clock(void);

#if LCD_READING_FUNCTIONALITY
static char lcdReadUpperNibble(void);
static char lcdReadLowerNibble(void);
#endif

void lcdInit(void)
{   
    /* Setup LCD pins as output - leave other pins as input */
    LCD_SET_PINS_OUTPUT();
    LCD_SET_PINS_LOW();

    LCD_STARTUP_WAIT();

    /*Set startup sequence to ensure correct initalisation of lcd*/
    /*First*/
    lcdSetUpperNibble(ST7066_START_UP_COMMAND);
    clock();
    TIME430_DELAY_MS(5UL);
    /*Second*/
    lcdSetUpperNibble(ST7066_START_UP_COMMAND);
    clock();
    TIME430_DELAY_US(200UL);
    /*Third*/
    lcdSetUpperNibble(ST7066_START_UP_COMMAND);
    clock();
    TIME430_DELAY_US(200UL);

    /*Set display to 4 bit mode*/
    LCD_SET_PINS_INSTRUCTION_WRITE();
    lcdSetUpperNibble(ST7066_FUNCTION_SET_BITS | ST7066_BIT_MODE_4);
    clock();
    LCD_COMMAND_WAIT();

    /*Set up LCD now that it is in 4 bit mode*/
    LCD_FUNCTION_SET(ST7066_BIT_MODE_4 | ST7066_LINE_MODE_2);
    LCD_CLEAR_DISPLAY();
    LCD_RETURN_HOME();
    LCD_ENTRY_MODE_SET(ST7066_MOVE_CURSOR_RIGHT); 
    LCD_DISPLAY_ON_OFF(ST7066_DISPLAY_ON);
}

/*Writes a null terminated string to the lcd.*/
void lcdPrint(const char const * string)
{
    int index = 0;

    while(string[index] != '\0')
    {
        lcdWriteDataToRam(string[index]);
        index++;
    }
}

/* Writes Instructions to the LCD i.e. RS and RW are low. 
 * The argument is the command as per data sheet. */
void lcdWriteInstruction(const char command)
{
    /*Setup - somewhat of a sanity setup*/
    LCD_SET_PINS_OUTPUT();
    LCD_SET_PINS_INSTRUCTION_WRITE();

    /*Clock Upper Nibble*/
    lcdSetUpperNibble(command);
    clock();
    /*Clocked Upper Nibble */

    LCD_COMMAND_WAIT();

    /*Clock Lower Nibble*/
    lcdSetLowerNibble(command);
    clock();
    /*Clocked Lower Nibble */

    LCD_COMMAND_WAIT();
}


void lcdWriteDataToRam(const char data)
{
    /*Setup - somewhat of a sanity setup*/
    LCD_SET_PINS_OUTPUT();
    LCD_SET_PINS_DATA_WRITE();

    /*Clocking Upper Nibble*/
    lcdSetUpperNibble(data);
    clock();
    LCD_DATA_WAIT();
    /*Upper Nibble Clocked*/

    /*Clocking Lower Nibble */
    lcdSetLowerNibble(data);
    clock();
    LCD_DATA_WAIT();
    /*Lower Nibble Clocked*/
}


/*Sets the correct data bits to upper nibble*/
static void lcdSetUpperNibble(const char data)
{
    /*Set data pins lows*/
    LCD_DATA_PINS_PORT_OUT &= ~LCD_DATA_PINS;

    if(data & BIT7)
    {
        LCD_DATA_PINS_PORT_OUT |=  LCD_DB7;
    }

    if(data & BIT6)
    {
        LCD_DATA_PINS_PORT_OUT |=  LCD_DB6;
    }

    if(data & BIT5)
    {
        LCD_DATA_PINS_PORT_OUT |=  LCD_DB5;
    }

    if(data & BIT4)
    {
        LCD_DATA_PINS_PORT_OUT |= LCD_DB4;
    }
}

/*Sets the correct data bits to lower nibble*/
static void lcdSetLowerNibble(const char data)
{
    /*Set data pins low*/
    LCD_DATA_PINS_PORT_OUT &= ~LCD_DATA_PINS;

    if(data & BIT3)
    {
        LCD_DATA_PINS_PORT_OUT |=  LCD_DB7;
    }

    if(data & BIT2)
    {
        LCD_DATA_PINS_PORT_OUT |=  LCD_DB6;
    }

    if(data & BIT1)
    {
        LCD_DATA_PINS_PORT_OUT |=  LCD_DB5;
    }

    if(data & BIT0)
    {
        LCD_DATA_PINS_PORT_OUT |=  LCD_DB4;
    }
}

/*Below here are functions to read from the LCD module.
  Set LCD_READING_FUNCTIONALITY in lcd.h to 0 to not 
  compile this*/
#if LCD_READING_FUNCTIONALITY
static char lcdReadUpperNibble(void)
{
    char data = 0x00;

    if(LCD_DATA_PINS_PORT_IN & LCD_DB7)
    {
        data |= BIT7;
    }

    if (LCD_DATA_PINS_PORT_IN & LCD_DB6)
    {
        data |= BIT6;
    }

    if(LCD_DATA_PINS_PORT_IN & LCD_DB5)
    {
        data |= BIT5;
    }

    if (LCD_DATA_PINS_PORT_IN & LCD_DB4)
    {
        data |= BIT4;
    }

    return data;
}


static char lcdReadLowerNibble(void)
{
    char data = 0x00;

    if(LCD_DATA_PINS_PORT_IN & LCD_DB7)
    {
        data |= BIT3;
    }

    if (LCD_DATA_PINS_PORT_IN & LCD_DB6)
    {
        data |= BIT2;
    }

    if(LCD_DATA_PINS_PORT_IN & LCD_DB5)
    {
        data |= BIT1;
    }

    if (LCD_DATA_PINS_PORT_IN & LCD_DB4)
    {
        data |= BIT0;
    }

    return data;
}

static void clock(void)
{
    LCD_SET_E_HIGH(); 
    LCD_E_WAIT();
    LCD_SET_E_LOW();
    LCD_E_WAIT();
}

/*TODO*/
char lcdReadDataFromRam(void)
{
    char data = 0x00;
    /*Setup*/
    LCD_SET_PINS_INPUT();
    LCD_SET_PINS_DATA_READ();

    /*Reading Upper Nibble*/
    LCD_SET_E_HIGH();
    LCD_E_WAIT();
    data = lcdReadUpperNibble();
    LCD_SET_E_LOW();
    LCD_DATA_WAIT();
    /*Upper Nibble Read*/

    /*Reading Lower Nibble*/
    LCD_SET_E_HIGH();
    LCD_E_WAIT();
    data |= lcdReadLowerNibble();
    LCD_SET_E_LOW();
    LCD_DATA_WAIT();
    /*Lower Nibble Read*/

    return data;
}

/*TODO*/
char lcdReadBusyFlagAndAddress(void)
{
    char data = 0x00;
    /*Setup*/
    LCD_SET_PINS_INPUT();
    LCD_SET_PINS_INSTRUCTION_READ();

    /*Reading Upper Nibble*/
    LCD_SET_E_HIGH();
    LCD_E_WAIT();
    data = lcdReadUpperNibble();
    LCD_SET_E_LOW();
    LCD_DATA_WAIT();
    /*Upper Nibble Read*/

    /*Reading Lower Nibble*/
    LCD_SET_E_HIGH();
    LCD_E_WAIT();
    data |= lcdReadLowerNibble();
    LCD_SET_E_LOW();
    LCD_DATA_WAIT();
    /*Lower Nibble Read*/

    return data;
}

#endif /*LCD_READING_FUNCTIONALITY*/


