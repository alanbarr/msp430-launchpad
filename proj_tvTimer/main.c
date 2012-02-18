/* 
** Author: Alan Barr
** Created: February 2012
** Compiler: mspgcc
*
* This project is still in development stage. The ultimate goal is a device
* which can act as a reminder for TV shows, and will change the channel just
* before a show comes on.
*/

#include "string.h"
#include "time430.h"
#include "msp430g2553.h"
#include "legacymsp430.h"

/*********************************** Macros ***********************************/

#define IR_LED                      (BIT1)

/* Deinations used for carrier wave */
#define CW_PERIOD_US                (25)
#define CW_PERIOD_CYCLES            (TIME430_US_CYCLES * CW_PERIOD_US)    
#define CW_HALF_PERIOD_CYCLES       (CW_PERIOD_CYCLES / 2) 

#define TIMER_VALUE_CLEARING        (40000)

/* Timings for transmission states */
#define CLEARING_TIME_US            (45000)
#define HEADER_STATE_TIME_US        (2400)
#define HIGH_STATE_TIME_US          (1200)
#define PAUSE_LOW_STATE_TIME_US     (600)
#define CLEARING_INT_INTERVAL_US    (TIMER_VALUE_CLEARING / TIME430_US_CYCLES)

/* Interrupts in various states */
#define INTS_IN_CLEARING_STATE      ((CLEARING_TIME_US / \
                                    CLEARING_INT_INTERVAL_US)+1) 
#define INTS_IN_HEADER_STATE        (HEADER_STATE_TIME_US / CW_PERIOD_US)
#define INTS_IN_PAUSE_LOW_STATE     (PAUSE_LOW_STATE_TIME_US  / CW_PERIOD_US)
#define INTS_IN_HIGH_STATE          (HIGH_STATE_TIME_US   / CW_PERIOD_US)

#define START_TX_INT_NUM            1

/* These codes are in format: 4 Unused bits + Device + Command, 
 * i.e. BIT0 is LSB */
#define COMMAND_POWER               0x0095      
/********************************* Prototypes *********************************/

void startTransmit(const unsigned int data);

/********************************* Data Types *********************************/

typedef struct { unsigned char hour; 
                 unsigned char minute; 
                 unsigned char date; 
                 unsigned char month;
                 unsigned char eventActive;
                 char eventName[10];
                } tReminderStructure;

typedef struct { unsigned int minutes;
                 unsigned int hours;
                 unsigned int date;
                 unsigned int month;
                 unsigned int year;
                } tTimeStructure;

typedef enum { entry    = 0x01,
               header   = 0x02,
               pause    = 0x04, 
               active   = 0x08,
               clearing = 0x10,
               exit     = 0x20,
            } eIrTransmissionState;


/***************************** TimeKeeping Globals*****************************/ 

volatile unsigned char gSeconds = 0;
volatile unsigned char gMinutes = 0;
volatile unsigned char gHours   = 0;

/********************************* IR GLOBALS *********************************/

const unsigned int sonyTransmissionCodes[] = { COMMAND_POWER,
                                               0x02,
                                               0x03,
                                             };


volatile unsigned int gNumberOfInterruptsToStateChange = 0;
volatile eIrTransmissionState gIrState;
volatile unsigned int irTransmitData = 0;

/********************************* Functions **********************************/

int main(void)
{
    tReminderStructure reminderList[10];
    tTimeStructure currentTime;

    WDTCTL = WDTPW | WDTHOLD;

    TIME430_CALIBRATE_CLOCK();

    memset(&currentTime, 0, sizeof(tTimeStructure));
    memset(reminderList, 0, sizeof(tReminderStructure) * 10);

    P1DIR = BIT0 | BIT6;
    P1OUT = 0x00;
    P2OUT = 0x00;
    
    while(1)
    {
        startTransmit(COMMAND_POWER);
    }
    _BIS_SR(LPM0_bits); 
    return 0;
}

/* Initiates a transmission of an IR code. */
void startTransmit(const unsigned int data)
{
    gIrState = entry;
    gNumberOfInterruptsToStateChange = START_TX_INT_NUM;
    irTransmitData = data;
    TA1CCR0  = CW_PERIOD_CYCLES;                        // ~25 uS            
    TA1CCR1  = CW_HALF_PERIOD_CYCLES;                   // ~12.5 uS
    TA1CTL   = TASSEL_2 | MC_1;                         // SMCLK, Up Mod,
    P2DIR   |= IR_LED;                                  // IR led pin output
    TA1CCTL1 = OUTMOD_7;                                // O/P Reset/Set
    TA1CCTL0 = CCIE;                                    // Enable CC0 interrupt
    _BIS_SR(GIE | LPM0_bits);                           // Enter LPM for TX
}



/* Handles the transmission of an IR code. The transmission is split into 
 * various "states". During each state the output led will set to either output
 * the carrier wave for IR or remain low. These states last for a calculated 
 * number of interrupts. The MCU will remain in a low power state during the 
 * transmission of an IR code.*/
interrupt(TIMER1_A0_VECTOR) TA1_0_ISR(void)
{
    static int bitsToTransmit = 16;
     
    /* If number of interrupts remaining is zero, process what to do, otherwise 
     * get out of here! */

    if (!(--gNumberOfInterruptsToStateChange))
    {   
        P2SEL &= ~IR_LED;                               // Stop LED output
        
        /* If there was a 600 uS wait previously, check next bit to TX and set 
         * active time appropriately.*/
        if (gIrState == pause)
        {
            if (irTransmitData & 0x01)
            {
                gIrState = active;
                gNumberOfInterruptsToStateChange = INTS_IN_HIGH_STATE;
            }

            else
            {
                gIrState = active;
                gNumberOfInterruptsToStateChange = INTS_IN_PAUSE_LOW_STATE;
            }
            
            /* Now that the on duration is set, turn on led, and count bit as 
             * transmitted. */
            irTransmitData >>= 1;                                                
            bitsToTransmit  -= 1;
            P2SEL |= IR_LED;                                                    
        }

        /* Otherwise the state to enter is 600 uS pause that precedes high and 
         * low transitions. */
        else if (gIrState == active || gIrState == header)
        {
            gIrState = pause;
            gNumberOfInterruptsToStateChange = INTS_IN_PAUSE_LOW_STATE;
        }

        /* If entering this function for the first time set variables to 
         * default. */
        else if (gIrState == entry)
        {
            bitsToTransmit = 16;
            gIrState = header;
            gNumberOfInterruptsToStateChange = INTS_IN_HEADER_STATE;
            P2SEL |= IR_LED;
        }
        
        if (!bitsToTransmit)
        {
            /* If in fact we have transmitted the command, enter clearing 
             * state which is a  45mS delay. This is handled in a TimerA count 
             * of 40,000 * the appropriate  number of interrupts for clock 
             * speed.*/
            if(gIrState != clearing)
            {
                P1OUT = BIT6;
                gIrState = clearing;
                TA1CCR0  = TIMER_VALUE_CLEARING;   
                TA1CCR1  = 0x00;
                TA1CCTL1 = 0x00;
                TA1R     = 0x00;
                gNumberOfInterruptsToStateChange = INTS_IN_CLEARING_STATE;
            }

            /* Clearing period has passed, reset timer disabling interrupts. 
             * Exit low power mode upon leaving ISR. */
            else
            {
                P1OUT = BIT0;
                TA1CTL   = 0x00;                                        
                TA1CCTL0 = 0x00;                                        
                _BIC_SR_IRQ(LPM0_bits);          
            }
        }
    }
}


