/* Author: Alan Barr 
** Created: Jan 2012 */
#include "msp430g2553.h"
#include "i2cBitExpanderLcd.h"
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    TIME430_CALIBRATE_CLOCK();
    P1DIR = BIT0;
    P1OUT &=~BIT0;
    __delay_cycles(100UL);
    __delay_cycles(100UL);
    
    i2cSetupPins();
    i2cLcdInit();
    
    i2cLcdPrint("Hello World!"); 
    
    /*Sleep*/
    __bis_SR_register(LPM4_bits); 
    return 0;
}
