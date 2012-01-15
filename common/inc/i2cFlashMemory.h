/*
* Author: Alan Barr 
* Created: 2011
*/

#ifndef I2C_FLASH_MEMORY_H_
#define I2C_FLASH_MEMORY_H_

#include "i2c.h"
#include <string.h>

void i2cFMWrite(const char * data, const char i2cAddress, const unsigned int internalAddress, 
                int numberOfBytes);
void i2cFMRandomRead(volatile char * data, const char i2cAddress, const unsigned int internalAddress,
                     const int numberOfBytes);


#endif /*I2C_FLASH_MEMORY_H_*/
