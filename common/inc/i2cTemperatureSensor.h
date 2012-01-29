/*
**Author: Alan Barr
**Created: 2012
*/

#ifndef I2C_TEMP_H_
#define I2C_TEMP_H_

#include "i2c.h"
#include "TMP102.h"

void i2cTSRead(const char tempAddress, char * buffer);
float i2cTSConvertTemp(char * buffer);

#endif /*I2C_TEMP_H_*/
