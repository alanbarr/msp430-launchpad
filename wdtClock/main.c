/*
 * A basic clock functionality using WDT.
 * Using a 6 pF C-001R crystal for ACLK.
 */
/* Copyright (C) Alan Barr 2012*/

#include "msp430g2553.h"
#include "legacymsp430.h"

/* Leds which toggle every second / minute. */
#define SEC_LED BIT0                                    // Launchpad red LED = seconds
#define MIN_LED BIT6                                    // Launchpad green LED = minutes

volatile unsigned char seconds = 0;
volatile unsigned char minutes = 0;
volatile unsigned char hours   = 0;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                           // Pause dog - for now
    
    BCSCTL1 = CALBC1_1MHZ;           
    DCOCTL  = CALDCO_1MHZ;

    P1OUT = 0x00;
    P2OUT = 0x00;

    P1DIR = SEC_LED + MIN_LED;                          // Set Leds to outputs

    WDTCTL = WDT_ADLY_1000;                             // Setup WDT for 1000ms interval interrupt - ACLK 
    IE1 |= WDTIE + OFIE;                                // Enable WDT interrupt and Osc Fault interrupt
    BCSCTL3 = XCAP_1;                                   // 6 pF. LP included crystal is 12.5 pF use XCAP_3        
   
    while(1)
    {
        if (seconds > 59)
        {
            minutes++;
            seconds = 0;
            P1OUT ^= MIN_LED;
        }

        if (minutes > 59)
        {
            hours++;
            minutes = 0;
        }

        if (hours > 23)
        {
            hours = 0;
        }
        
        __bis_SR_register(LPM3_bits + GIE);
    }
    
    return 0;
}

/* WDT Interrupt */
interrupt(WDT_VECTOR) wdtInterrupt(void)
{
    seconds++;
    P1OUT ^= SEC_LED;
    
    __bic_status_register_on_exit(LPM3_bits);
}

/* Non Maskable Interrupt Handler - Osc Fault
   Osc Faults can occur just by having your hand close to crystal */ 
interrupt(NMI_VECTOR) nmiHandler (void)
{
    unsigned volatile int delayCtr = 0;
    while (IFG1 & OFIFG)    
    {
        IFG1 &= ~OFIFG;                                 // Clear Oscillator fault flag
        P1OUT = BIT0 + BIT6;                            // Light LEDs for warning 
        for (delayCtr=0;delayCtr<0xFF;delayCtr++);      // Delay for fault to clear
    }

    /* Reset time.*/
    seconds = 0;
    minutes = 0;
    hours   = 0;

    P1OUT = 0x00;                                              
    IE1 |= OFIE;                                        // NMI's get cleared - re enable
}
