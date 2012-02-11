/*
** Author: Alan Barr 
** Created: 2011
** Compiler: mspgcc
**
** A basic clock functionality using WDT.
** Using a 6 pF C-001R crystal for ACLK.
*/
#include "msp430g2553.h"
#include "legacymsp430.h"
#include "lcd.h"
#include "time430.h"

/* Leds which toggle every second / minute. */
#define SEC_LED         BIT0                        // Launchpad red LED = seconds
#define MIN_LED         BIT6                        // Launchpad green LED = minutes

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
    WDTCTL = WDTPW + WDTHOLD;
    
    TIME430_CALIBRATE_CLOCK();
    
    BCSCTL3 = XCAP_1;                               // 6 pF. LP included crystal is 12.5 pF use XCAP_3        

    /*Configure Timer A*/
    TA0CCR0 = 0x7FFF;                               // Watch crystal frequency - 1 second interrupt
    TA0CCTL0 = CCIE;
    TA0CTL  = TASSEL_1 | MC_1 | ID_0 ;              // ACLK, Up to TACCR0, Div 0, interrupt

    /*Setup leds*/
    P1OUT = 0x00;
    P2OUT = 0x00;
    P1DIR = SEC_LED + MIN_LED;                      // Set Leds to outputs

    lcdInit();

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
        
        updateDisplay();
        __bis_SR_register(LPM3_bits + GIE);
    }
    
    return 0;
}


/*Timer interrupt handler. Updates second count*/
interrupt(TIMER0_A0_VECTOR) Timer0_A0_ISR(void)
{
    timeArray[SECONDS]++;
    P1OUT ^= SEC_LED;
    __bic_status_register_on_exit(LPM3_bits);
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


