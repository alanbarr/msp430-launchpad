/* Author: Alan Barr
 * Created: May 2012 
 * Compiler: mspgcc
 
 * Small program to demonstrate using the switch on the launchpad.
 * Each time the switch is pressed the launchpad LEDs should toggle between 
 * values. The switch is polled for a change.
 */

#include "msp430g2553.h"

#define SWITCH              BIT3
#define SWITCH_PRESSED      0x00

#define RED                 BIT0
#define GREEN               BIT6 

int main(void)
{
    /* Disabling dog, setting up clock */
    WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    /* Setting up Switch */
    P1DIR &= ~SWITCH;                               // Set the switch pin to input
    P1REN |= SWITCH;                                // Use an internal resistor
    P1OUT |= SWITCH;                                // The internal resistor is pullup

    /* Setting up Launchpad LEDs */
    P1DIR |= RED + GREEN;                           // Launchpad LEDs for output
    P1OUT |= GREEN;                                 // Green LED lit at startup
    P1OUT &= ~RED;                                  // Red LED off at startup


    while (1)
    {
        if ((P1IN & SWITCH) == SWITCH_PRESSED)
        {
            /* Hang until the switch is released. */
            while ((P1IN & SWITCH) == SWITCH_PRESSED); 

            P1OUT ^= GREEN | RED;                   // Toggle Launchpad LEDs
        }
    }

    return 0;
}
