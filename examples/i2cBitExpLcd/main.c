/* Author: Alan Barr 
** Created: Jan 2012
** Compiler: mspgcc
*/
#include "msp430g2553.h"
#include "i2cBitExpanderLcd.h"
#include "time430.h"
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    TIME430_CALIBRATE_CLOCK();
    P1DIR = BIT0;
    P1OUT &=~BIT0;
    TIME430_DELAY_MS(100UL);
    TIME430_DELAY_MS(100UL);
    
    i2cSetupPins();
    i2cLcdInit();
    
    i2cLcdPrint("Hello World!"); 

    LCD_DDRAM_ADDRESS_SET(ST7066_LINE1_POS0);
    i2cLcdPrint("MSP-LAUNCHPAD");
    
    /*Sleep*/
    __bis_SR_register(LPM4_bits); 
    return 0;
}
