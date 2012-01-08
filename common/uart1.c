/*
* Copyright (C)
* Originally by: D. Dang, Texas Instruments Inc. December 2010
* Modified by: Alan Barr 2011
*/

#include "uart1.h"

static int uartTxData1;                        				            // UART internal variable for TX
volatile char uart1RxBuffer[UART1RXBUFSIZE];                            // Array to save rx'ed characters
volatile unsigned int uart1RxBufferIndex = 0;                           // Index to uart1RxBuffer

void uart1SetupPins(void)
{
	P2OUT &= ~(UART1_TXD + UART1_RXD); 
	P2SEL |= UART1_TXD + UART1_RXD;            							// Timer function for TXD/RXD pins
	P2DIR |= UART1_TXD;             								    // Set all pins but RXD to output
    P2DIR &= ~UART1_RXD;
}


//------------------------------------------------------------------------------
// Function configures Timer_A for full-duplex UART operation
//------------------------------------------------------------------------------
void uart1Init(void)
{
	__enable_interrupt();
    TA1CCTL0 = OUT;                          							// Set TXD Idle as Mark = '1'
    TA1CCTL1 = SCS + CM1 + CAP + CCIE;       							// Sync, Neg Edge, Capture, Int
    TA1CTL = TASSEL_2 + MC_2;               							// SMCLK, start in continuous mode
}

void uart1Disable(void)
{
	__disable_interrupt();
    TA1CCTL0 = 0x00;                          							
    TA1CCTL1 = 0x00;       							
    TA1CTL = 0x00;               							 
}


//------------------------------------------------------------------------------
// Timer_A UART - Receive Interrupt Handler
//------------------------------------------------------------------------------
//#pragma vector = TIMER1_A1_VECTOR
//__interrupt void Timer1_A1_ISR(void)
interrupt(TIMER1_A1_VECTOR) Timer1_A1_ISR(void)
{
    static unsigned char rxBitCnt = 8;
    static unsigned char rxData = 0;

    switch (TA1IV)  					
 	{
        case TA1IV_TACCR1:                    							// TACCR1 CCIFG - UART RX
            TA1CCR1 += UART1_TBIT;                 						// Add Offset to CCRx
           	if (TA1CCTL1 & CAP)                  						// Capture mode = start bit edge
            {
                TA1CCTL1 &= ~CAP;                 						// Switch capture to compare mode
                TA1CCR1 += UART1_TBIT_DIV_2;       						// Point CCRx to middle of D0
            }
            else 
            {
                rxData >>= 1;
                if (TA1CCTL1 & SCCI)             			 		   	// Get bit waiting in receive latch
                {  
                    rxData |= 0x80;
                }
                rxBitCnt--;
               	if (rxBitCnt == 0)              						// All bits RXed?
                {
                    uart1RxBuffer[uart1RxBufferIndex] = rxData;        	// Store in global variable
                    if (uart1RxBufferIndex < UART1RXBUFSIZE)
                    {
                        uart1RxBufferIndex++;
                    }
                    rxBitCnt = 8;                						// Re-load bit counter
                    TA1CCTL1 |= CAP;              						// Switch compare to capture mode
                    __bic_SR_register_on_exit(LPM0_bits);  				// Clear LPM0 bits from 0(SR)
                }
            }
            break;
    	}
}
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
// Outputs one byte using the Timer_A UART
//------------------------------------------------------------------------------
void uart1Tx(char byte)
{
    while (TA1CCTL0 & CCIE);                 							// Ensure last char got TX'd
    TA1CCR0 = TAR;                           							// Current state of TA counter
    TA1CCR0 += UART1_TBIT;                    							// One bit time till first bit
    TA1CCTL0 = OUTMOD0 + CCIE;               							// Set TXD on EQU0, Int
    uartTxData1 = byte;                          						// Load global variable
    uartTxData1 |= 0x100;                        					    // Add mark stop bit to TXData
    uartTxData1 <<= 1;                           					    // Add space start bit
}

//------------------------------------------------------------------------------
// Prints a string over using the Timer_A UART
//------------------------------------------------------------------------------
void uart1Print(char *string)
{
    while (*string != '\0') 
    {
        uart1Tx(*string++);
    }
}

//------------------------------------------------------------------------------
// Timer_A UART - Transmit Interrupt Handler
//------------------------------------------------------------------------------
//#pragma vector = TIMER1_A0_VECTOR
//__interrupt void Timer1_A0_ISR(void)
interrupt(TIMER1_A0_VECTOR) Timer1_A0_ISR(void)
{
    static  char txBitCnt = 10;

    TA1CCR0 += UART1_TBIT;                    							// Add Offset to CCRx
    if (txBitCnt == 0)                     								// All bits TXed?
    {    
    	TA1CCTL0 &= ~CCIE;                   							// All bits TXed, disable interrupt
        txBitCnt = 10;                      							// Re-load bit counter
    }
    else 
    {
        if (uartTxData1 & 0x01) 
        {
          TA1CCTL0 &= ~OUTMOD2;              							// TX Mark '1'
        }
        else 
        {
          TA1CCTL0 |= OUTMOD2;               							// TX Space '0'
        }
        uartTxData1 >>= 1;
        txBitCnt--;
    }
}      

