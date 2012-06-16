#include "i2cTemperatureSensor.h"

void i2cTSRead(const char tempAddress, char * buffer)
{
    /* Move Temperature Sensor pointer to temperature register */
    i2cSetupTx(tempAddress);
    i2cTransmit(TMP102_TEMPERATURE_REGISTER,1);
    i2cPoll(tempAddress);
    /*Receive 2 Bytes of temperature data*/
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
    temperature_f = (float)temperature * TMP102_CONVERSION;    
    
    return temperature_f;
    
}

