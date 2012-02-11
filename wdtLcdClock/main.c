/*
** Author: Alan Barr 
** Created: 2011
** Compiler: mspgcc
**
** A basic clock functionality using WDT and an LCD display.
** Using a 6 pF C-001R crystal for ACLK.
*/
#include "msp430g2553.h"
#include "legacymsp430.h"
#include "lcd.h"
#include "time430.h"

/* Leds which toggle every second / minute. */
#define SEC_LED         BIT0                // Launchpad red LED = seconds
#define MIN_LED         BIT6                // Launchpad green LED = minutes
/*Indexs to time array*/
#define HOURS           0
#define MINUTES         1
#define SECONDS         2
/*Indexs to string array*/
#define SECONDS_UNITS   7
#define SECONDS_TENS    6
#define M_S_COLON       5
#define MINUTES_UNITS   4
#define MINUTES_TENS    3
#define H_M_COLON       2
#define HOURS_UNITS     1
#define HOURS_TENS      0

void updateDisplay(void);

volatile unsigned char timeArray[3] = {0x00};

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                           // Pause dog - for now
    TIME430_CALIBRATE_CLOCK();

    P1OUT = 0x00;
    P2OUT = 0x00;
    P1DIR = SEC_LED + MIN_LED;                          // Set Leds to outputs

    lcdInit();

    WDTCTL = WDT_ADLY_1000;                             // Setup WDT for 1000ms interval interrupt - ACLK 
    IE1 |= WDTIE + OFIE;                                // Enable WDT interrupt and Osc Fault interrupt
    BCSCTL3 = XCAP_1;                                   // 6 pF. LP included crystal is 12.5 pF use XCAP_3        
   
    while(1)
    {
        if (timeArray[SECONDS] > 59)
        {
            timeArray[MINUTES]++;
            timeArray[SECONDS] = 0;
            P1OUT ^= MIN_LED;
        }

        if (timeArray[MINUTES] > 59)
        {
            timeArray[HOURS]++;
            timeArray[MINUTES] = 0;
        }

        if (timeArray[HOURS] > 23)
        {
            timeArray[HOURS] = 0;
        }
        
        /*Update LCD display*/
        updateDisplay();
            
        __bis_SR_register(LPM3_bits + GIE);
    }
    
    return 0;
}

/* WDT Interrupt */
interrupt(WDT_VECTOR) wdtInterrupt(void)
{
    timeArray[SECONDS]++;
    P1OUT ^= SEC_LED;
    
    __bic_status_register_on_exit(LPM3_bits);
}

/* Non Maskable Interrupt Handler - Osc Fault
   Osc Faults can occur just by having your hand close to crystal */ 
interrupt(NMI_VECTOR) nmiHandler (void)
{
    while (IFG1 & OFIFG)    
    {
        IFG1 &= ~OFIFG;                                 // Clear Oscillator fault flag
        P1OUT = BIT0 + BIT6;                            // Light LEDs for warning 
        TIME430_DELAY_MS(5);
    }

    /* Reset time.*/
    timeArray[SECONDS] = timeArray[MINUTES] = timeArray[HOURS] = 0;

    P1OUT = 0x00;                                              
    IE1 |= OFIE;                                        // NMI's get cleared - re enable
}

void updateDisplay(void)
{
    unsigned char tempTime = 0x00;
    int timeArrayIndex = HOURS;
    int timeStringIndex = HOURS_UNITS;
    char timeString[9] = {0x00};

    timeString[H_M_COLON] = timeString[M_S_COLON] = ':';

    while (timeArrayIndex < 3)
    {
        tempTime = timeArray[timeArrayIndex];
        timeString[timeStringIndex--] = tempTime % 10 + 0x30;       // Units
        timeString[timeStringIndex] = tempTime / 10 + 0x30;         // Tens
        timeStringIndex+=4;                                         // Move to next unit
        timeArrayIndex++;
    }

    LCD_RETURN_HOME();
    lcdPrint(timeString);
}
