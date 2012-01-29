#define                 TIME430_CLOCK_FREQ 1
#define BITEXPANDER     MCP23017_ADDRESS

#include "MCP23017.h"
#include "i2c.h"
#include "msp430g2553.h"
#include "time430.h"

int main(void)
{
    char TxData[4] = {MCP23017_IODIRA, 0x00, MCP23017_GPIOA, 0xFF};
    int ctr = 0;
    int ctr1 =0; 
    WDTCTL = WDTPW + WDTHOLD;
    TIME430_CALIBRATE_CLOCK();

    i2cSetupPins();
    P1DIR = BIT0;
    P1OUT &= ~BIT0;

    i2cSetupTx(BITEXPANDER);
    i2cTransmit(&TxData[0],2);
    
    while(1)
    {
        i2cTransmit(&TxData[2],2);  
        for(ctr=15; ctr>=0; ctr--)
        {
            TxData[3] = (char)ctr;

            for(ctr1=0; ctr1<100; ctr1++)
            {
                TIME430_DELAY_MS(10);
            }
            i2cTransmit(&TxData[2],2);  
        }
    }
    
    return 0;  
}


