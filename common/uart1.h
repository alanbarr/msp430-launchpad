/*
* Copyright (C)
* Originally by: D. Dang, Texas Instruments Inc. December 2010
* Modified by: Alan Barr 2011
*/

#ifndef UART1_H_
#define UART1_H_

#include <msp430.h>
#include <legacymsp430.h>

#define UART1RXBUFSIZE    10 

#define UART1_TXD       0x01                            //2.0 TX
#define UART1_RXD       0x02                            //2.1 RX

#define UART1_TBIT_DIV_2     (1000000 / (9600 * 2))     // (CLOCK / (UART BIT RATE *2))
#define UART1_TBIT           (1000000 / 9600)           // (CLOCK / UART BIT RATE)

extern volatile unsigned int uart1RxBufferIndex;
extern volatile char uart1RxBuffer[UART1RXBUFSIZE];             

void uart1SetupPins(void);
void uart1Init(void);
void uart1Tx(char byte);
void uart1Print(char *string);
void uart1Disable(void);

#endif /*UART1_H_*/
