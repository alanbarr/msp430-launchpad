/* Author: Alan Bar 
** Created: Feb 2012 
** Compiler: mspgcc
*/
#include "msp430g2553.h"
#include "lcd_st7066.h"
int main(void)
{
    char readChar = 0x00;
    WDTCTL = WDTPW + WDTHOLD;
    TIME430_CALIBRATE_CLOCK();
    lcdInit();
    lcdPrint("Hello World!"); 

    LCD_DDRAM_ADDRESS_SET(ST7066_LINE1_POS0);
    lcdPrint("MSP-LAUNCHPAD");

    /*Read first character on second line (M)*/
    LCD_DDRAM_ADDRESS_SET(ST7066_LINE1_POS0);
    readChar = lcdReadDataFromRam();
    
    /*Put read character into last place on second line*/
    LCD_DDRAM_ADDRESS_SET(ST7066_LINE1_POS15);
    lcdWriteDataToRam(readChar);
    
    /*Sleep*/
    __bis_SR_register(LPM4_bits); 
    return 0;
}
