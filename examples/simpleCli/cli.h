/*
** Author: Alan Barr 
** Created: 2011
*/

/* Builds with mspgcc */

#ifndef CLI_H_
#define CLI_H_

#include "time430.h"
#include "msp430g2553.h"
#include "uart.h"
#include "usefulFunctions.h"            
#include "commandFunctions.h"

#define LED1 0x01
#define LED2 0x40

#define UARTBUFSIZE     (20)
#define PRINT           uartPrint
#define PROMPT          "\n\r$"

#endif /*CLI_H_*/
