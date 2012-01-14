/*
* Copyright (C)
* Originally by: D. Dang, Texas Instruments Inc. December 2010
* Modified by: Alan Barr 2011
*/

#include "i2c.h"

static char i2cPolling=0;                                               //Polling i2c 
static int i2cRXByteCtr=0;
static char const *pI2cTxData;                                          // Pointer to TX data
static char *pI2cRxData;                                                // Pointer to i2cRX data
static char i2cTXByteCtr;
static char i2cRX = 0;
unsigned int i2cNacks = 0;
int i2cNoi =0;

void i2cSetupPins(void)
{
    P1SEL |= BIT6 + BIT7;                                               // Assign I2C pins to USCI_B0
    P1SEL2|= BIT6 + BIT7;                                               // Assign I2C pins to USCI_B0
}

void i2cSetupTx(const char address)
{
    dint();
    i2cRX = 0;
    IE2 &= ~UCB0RXIE;  
    while (UCB0CTL1 & UCTXSTP);                                         // Ensure stop condition got sent// Disable i2cRX interrupt
    UCB0CTL1 |= UCSWRST;                                                // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;                               // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;                                      // Use SMCLK, keep SW reset
    UCB0BR0 = 12;                                                       // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0I2CSA = address;                                                
    UCB0CTL1 &= ~UCSWRST;                                               // Clear SW reset, resume operation
    UCB0I2CIE |= UCNACKIE;
    IE2 |= UCB0TXIE;                                                    // Enable TX interrupt
}

void i2cSetupRx(const char address)
{
    dint();
    i2cRX = 1;
    IE2 &= ~UCB0TXIE;  
    UCB0CTL1 |= UCSWRST;                                                // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;                               // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;                                      // Use SMCLK, keep SW reset
    UCB0BR0 = 12;                                                       // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0I2CSA = address;                                                
    UCB0CTL1 &= ~UCSWRST;                                               // Clear SW reset, resume operation
    UCB0I2CIE |= UCNACKIE;
    IE2 |= UCB0RXIE;                                                    // Enable i2cRX interrupt
}

void i2cTransmit(const char * data, const int numberOfBytes)
{
    pI2cTxData = data;                                                  // TX array start address
    i2cTXByteCtr = numberOfBytes;                                       // Load TX byte counter
    while (UCB0CTL1 & UCTXSTP);                                         // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;                                         // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);                                    // Enter LPM0 w/ interrupts
}

void i2cReceive(volatile char * data, const int numberOfBytes)
{
    pI2cRxData = (char *)data;                                          // Start of i2cRX buffer
    i2cRXByteCtr = numberOfBytes;                                       // Load i2cRX byte counter
    while (UCB0CTL1 & UCTXSTP);                                         // Ensure stop condition got sent
    UCB0CTL1 |= UCTXSTT;                                                // I2C start condition
    while (UCB0CTL1 & UCTXSTT);
    __bis_SR_register(CPUOFF + GIE);                                    // Enter LPM0 w/ interrupts
}

void i2cPoll(char address)
{   
    int ctr;
    for(ctr=0;ctr<2000;ctr++);
    /*
    do                          //i2cPolling for write complete
    {
        i2cPolling = 1;
        Setup_TX(address);
        i2cTransmit(0,1);       //DEBUG 
    }   
    while(i2cPolling == 2);
    i2cPolling = 0; 
    */
}

interrupt(USCIAB0RX_VECTOR) USCIAB0RX_ISR(void)
{   
    if ((UCB0STAT & UCNACKIFG) == UCNACKIFG && i2cPolling == 1)
    {
        i2cNacks++;
        UCB0STAT &= ~UCNACKIFG;
        UCB0CTL1 |= UCTXSTP;
        while(UCB0CTL1 & UCTXSTP);
        i2cPolling = 2;
        __bic_SR_register_on_exit(CPUOFF); 
    
    }
    else
    {
        UCB0CTL1 |= UCTXSTP;
        while(UCB0CTL1 & UCTXSTP);
        while(1);
    }
}



interrupt(USCIAB0TX_VECTOR) USCIAB0TX_ISR(void)
{
    if(i2cRX == 1)                                                          // Master Recieve?
    {
        i2cNoi++;
        i2cRXByteCtr--;                                                     // Decrement i2cRX byte counter
        if (i2cRXByteCtr > 0)
        {
            *pI2cRxData++ = UCB0RXBUF;                                      // Move i2cRX data to address pI2cRxData
        }
        else if(i2cRXByteCtr == 0)
        {
                IE2 &= ~ UCB0TXIE;      //DEBUG disable interrupt
                IFG2 &= ~UCB0TXIFG; 
                UCB0CTL1 |= UCTXSTP;   
                *pI2cRxData++ = UCB0RXBUF;                                  // I2C stop condition
                while (UCB0STAT & UCBBUSY);
                UCB0CTL1 |= UCSWRST;                                        // Move final i2cRX data to pI2cRxData
                 __bic_SR_register_on_exit(CPUOFF);     
        }
    }
    else
    {                                                                       // Master Transmit
        if (i2cTXByteCtr)                                                   // Check TX byte counter
        {
          UCB0TXBUF = *pI2cTxData++;                                        // Load TX buffer
          i2cTXByteCtr--;                                                   // Decrement TX byte counter
        }
        else
        {
            UCB0CTL1 |= UCTXSTP;                                            // I2C stop condition
            IFG2 &= ~UCB0TXIFG; 
            __bic_SR_register_on_exit(CPUOFF);                              // Exit LPM0
        }
    }

}


    
