#include "i2cTemperatureSensor.h"
void i2cTSRead(const char tempAddress, char * buffer)
{
	i2cSetupTx(tempAddress);
	i2cTransmit(TEMPREG,1);
	i2cPoll(tempAddress);
	i2cSetupRx(tempAddress);
	i2cReceive(buffer, 2);
    UCB0CTL1 |= UCSWRST;
}

float i2cTSConvertTemp(char * buffer)
{
	float temperature_f = 0;
	int temperature = 0;
	temperature = buffer[0] << 4;
    temperature |= buffer[1] >> 4;
    temperature_f = (float)temperature * 0.0625;	
	
	return temperature_f;
	
}

