/*
**  Author: Alan Barr 
**  Created: 2011
**
**  Written for mspgcc
**  Visualises current light level measured from an LDR using ADC
**  to a bi-colour LED. The LED transitions from one colour to 
**  another depending on light level using PWM. Pressing switch
**  1.3 on the launchpad will change which colour represents high
**  and low light levels.
**  
**  1.3 - Launchpad Switch             2.1 - Bi Colour LED Red Leg
**                                           Bi Colour LED GND - Launchpad GND
**                                     2.4 - Bi Colour LED Green Leg
**       VCC
**        |
**       LDR
**  1.4 - | 
**       56 kohm Resistor 
**        |
**       GND
*/

#include "msp430g2553.h"
#include "legacymsp430.h"

/*Port 1 Pins*/
#define LAUNCHPAD_RED       BIT0
#define LAUNCHPAD_GREEN     BIT6

#define SWITCH              BIT3
#define LDR                 BIT4

/*Port 2 Pins*/
#define RED                 BIT1
#define GREEN               BIT4 

/*Registers for LED PWM*/
#define RED_PWM             TA1CCR1
#define GREEN_PWM           TA1CCR2  

/*Initial light configuration for LDR*/
#define LDR_DARK            500
#define LDR_LIGHT           600

/*Register value for IN register & SWITCH*/ 
#define SWITCH_PRESSED      0x00     

static unsigned int adcConversion = 0;
unsigned int minimumLdrValue = LDR_DARK;
unsigned int maximumLdrValue = LDR_LIGHT;

int main(void)
{
    unsigned int rangeLdrValues = maximumLdrValue - minimumLdrValue;

    WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    P1DIR = 0xFF;                                           // Ports Set to Output
    P2DIR = 0xFF; 
    P1OUT = 0x00;
    P2OUT = 0x00;

    /*Setting up Launchpad LEDs*/
    P1DIR |= LAUNCHPAD_RED + LAUNCHPAD_GREEN;               // Using Launchpad LEDs for "mode"
    P1OUT |= LAUNCHPAD_GREEN;                               // Inital Mode light = green

    /*Setting up Switch*/
    P1DIR &= ~SWITCH;                                       // Switch Input
    P1REN |= SWITCH;                                        // Resistor
    P1OUT |= SWITCH;                                        // Pullup
    P1IE   = SWITCH;                                        // Interrupt enable for Switch
    P1IES  = 0x00;                                          // Low to High Interrupt - Button Release
    P1IFG  = 0x00;                                          // Clear interrupt flags

    /*Setting up Timer A1 for PWM*/
    P2DIR = RED + GREEN;                                    // Setting up pins for Timer out 
    P2SEL = RED + GREEN;
    TA1CCTL0 = OUTMOD_7;                                   
    TA1CCTL1 = OUTMOD_3;                                    // Set/Reset - Red
    TA1CCTL2 = OUTMOD_7;                                    // Reset/Set - Green
    TA1CTL = TASSEL_2 + MC_1 + ID_0;                        // SMCLK, up mode, div 0

    /*Setting up ADC for LDR*/ 
    P1DIR    &= ~LDR;                                       // LDR Input
    ADC10CTL0 = MSC + ADC10SHT_3 + ADC10IE + ADC10ON;       // Multiple samples, 64 x ADC10CLKs, interrupt, on
    ADC10CTL1 = LDR << 11;                                  // Analogue Channel for LDR
    ADC10AE0  = LDR;                                        // Interupt for LDR
    
    while(1)
    {
        ADC10CTL0 |= ADC10SC + ENC;
        __bis_SR_register(CPUOFF + GIE);

        while(ADC10CTL1 & ADC10BUSY);

        adcConversion = ADC10MEM;
        
        if (adcConversion > maximumLdrValue)
        {
           maximumLdrValue = adcConversion; 
        }

        else if (adcConversion < minimumLdrValue)
        {
            minimumLdrValue = adcConversion;
        }
        
        rangeLdrValues = maximumLdrValue - minimumLdrValue;
        
        adcConversion -= minimumLdrValue;

        /*Divide by 10 to reduce period. Stops LED from appearing to blink*/
        TA1CCR0   = rangeLdrValues / 10;
        RED_PWM   = adcConversion  / 10;
        GREEN_PWM = adcConversion  / 10;
    }

    return 0;
}

/* ADC interrupt */
interrupt(ADC10_VECTOR) ADC10_ISR(void)
{
    __bic_SR_register_on_exit(CPUOFF);                      // Clear CPUOFF bit - ADC processing in main
}


/* Port 1 interrupt */
interrupt(PORT1_VECTOR) PORT1_ISR(void)
{
    volatile unsigned int delayCtr = 0;

    if (P1IFG & SWITCH)
    {
        while((P1IN & SWITCH) == SWITCH_PRESSED);           // While Button is held
        for(delayCtr=0; delayCtr<50; delayCtr++);           // Debouncing delay.  
        
        /*Swap Mode for Timer Pins - Swaps colours by altering between OUTMOD_3 and
        OUTMOD_7 - i.e. set/reset and reset/set*/
        TA1CCTL1 ^= OUTMOD2;                                
        TA1CCTL2 ^= OUTMOD2; 

        /*Reset Stored Min and Max Values*/
        minimumLdrValue = LDR_DARK;
        maximumLdrValue = LDR_LIGHT;

        P1OUT ^= LAUNCHPAD_GREEN + LAUNCHPAD_RED;           // Shows "mode" on launchpad leds
        P1IFG  &= ~(SWITCH);                                // Clear SWITCH interrupt flag 
        
    }
    __bic_SR_register_on_exit(CPUOFF);                      // Clear CPUOFF bit
}
