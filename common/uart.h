/*
* Copyright (C)
* Originally by: D. Dang, Texas Instruments Inc. December 2010
* Modified by: Alan Barr 2011
*/

#ifndef UART_H_
#define UART_H_

#include "legacymsp430.h"
#include "msp430.h"

#define UARTBUFSIZE     10

#define UART_TXD        0x02                     			// TXD on P1.1 (Timer0_A.OUT0)
#define UART_RXD        0x04                     			// RXD on P1.2 (Timer0_A.CCI1A)

#define UART_TBIT_DIV_2     (1000000 / (9600 * 2))
#define UART_TBIT           (1000000 / 9600)

extern volatile unsigned int uartRxBufferIndex;
extern volatile char uartRxBuffer[UARTBUFSIZE];                             		

void uartSetupPins(void);
void uartInit(void);
void uartTx(char byte);
void uartPrint(char *string);
void uartDisable(void);
void uartZeroBufferIndex(void);
#endif /*UART_H_*/
