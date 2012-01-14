/*
* Copyright (C)
* Originally by: D. Dang, Texas Instruments Inc. December 2010
* Modified by: Alan Barr 2011
*/

#ifndef I2C_H_
#define I2C_H_

#include "msp430.h"
#include <string.h>
#include <legacymsp430.h>

#define TEMPREG 0x00
#define FLASH 0x50

extern char i2cPolling;
extern int i2cRXByteCtr;
extern char *pI2cTxData;          
extern char *pI2cRxData;    
extern char i2cTXByteCtr;
extern char i2cRX;
extern unsigned int i2cNacks;
extern int i2cNoi;


void Setup_TX(char address);
void Setup_RX(char address);

void setupI2cPins(void);
void i2cTransmit(char * data, int numberOfBytes);
void i2cReceive(volatile char * data, int numberOfBytes);
void i2cFlashWrite(char * data, char i2cAddress, unsigned int internalAddress, int numberOfBytes);
void i2cRandomRead(volatile char * data, char i2cAddress, unsigned int internalAddress, int numberOfBytes);
void i2cPoll(char address);
void i2cTempRead(char tempAddress, char * buffer);
float i2cConvertTemp(char *buffer);

#endif /*I2C_H_*/
