/*
* Copyright (C)
* Originally by: D. Dang, Texas Instruments Inc. December 2010
* Modified by: Alan Barr 2011
* Compiler: mspgcc
*/

#ifndef I2C_H_
#define I2C_H_

#include "msp430.h"
#include <legacymsp430.h>

void i2cSetupTx(char address);
void i2cSetupRx(char address);
void i2cSetupPins(void);
void i2cTransmit(const char * data, const int numberOfBytes);
void i2cReceive(volatile char * data, const int numberOfBytes);
void i2cPoll(const char address);
#endif /*I2C_H_*/
