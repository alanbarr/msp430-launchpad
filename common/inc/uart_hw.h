#ifndef _UART_HW_H_
#define _UART_HW_H_

#include "legacymsp430.h"
#include "msp430.h"
#include "time430.h"
    
extern volatile unsigned int uartRxBufferIndex;

void uartSetupPins(void);
void uartInit(volatile char * pBuffer, volatile unsigned char sizeOfBuffer);
void uartTx(const char byte);
void uartPrint(const char *string);
void uartDisable(void);

#define UART_HW_BAUD 9600

#endif /*_UART_HW_H_*/
