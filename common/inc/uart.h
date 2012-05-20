/*
* Copyright (C)
* Originally by: D. Dang, Texas Instruments Inc. December 2010
* Modified by: Alan Barr 2011
*/

#ifndef UART_H_
#define UART_H_

#include "legacymsp430.h"
#include "msp430.h"
#include "time430.h"                                       
    
#define UART_TXD        0x02                                // TXD on P1.1 (Timer0_A.OUT0)
#define UART_RXD        0x04                                // RXD on P1.2 (Timer0_A.CCI1A)

#define UART_TBIT           (TIME430_CLOCK_HZ / 9600)
#define UART_TBIT_DIV_2     (UART_TBIT / 2)

extern volatile unsigned int uartRxBufferIndex;

void uartSetupPins(void);
void uartInit(volatile char * pBuffer, volatile unsigned char sizeOfBuffer);
void uartTx(const char byte);
void uartPrint(const char *string);
void uartDisable(void);
#endif /*UART_H_*/
