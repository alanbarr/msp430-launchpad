/*
* Author: Alan Barr 
* Created: 2011
*/

#include "i2cFlashMemory.h"

void i2cFMWrite(const char * data, const char i2cAddress, unsigned int internalAddress,
                int numberOfBytes)
{                                                       
    char TxData[66];
    memset((void *)&TxData[0],0x0,66 * sizeof(char));

    while(numberOfBytes > 0)
    {   
        TxData[0] = (char)((internalAddress >> 8) & 0xff);              //Placing internal address into transmit data
        TxData[1] = (char)(internalAddress & 0xff);
    
        i2cSetupTx(i2cAddress);
        
        if(numberOfBytes > 64)                                          //Sends 64 bytes of data
        {   
            memcpy(&TxData[2], data, 64);           
            i2cTransmit(TxData,66);
            numberOfBytes -= 64;
            data+=64;
            internalAddress += 64;
            i2cPoll(i2cAddress);    
        }
        else
        {
            memcpy(&TxData[2], data, numberOfBytes);
            i2cTransmit(TxData, numberOfBytes + 2);
            i2cPoll(i2cAddress);
            break;
        }   
    }
}

void i2cFMRandomRead(volatile char * data, const char i2cAddress,  const unsigned int internalAddress, 
                     const int numberOfBytes)
{   
    char internalAddressArray[2];
    internalAddressArray[0] = (char)((internalAddress >> 8) & 0xFF);
    internalAddressArray[1] = (char)(internalAddress & 0xFF);
    i2cSetupTx(i2cAddress);
    i2cTransmit(&internalAddressArray[0], 2);
    i2cPoll(FLASH);
    while (UCB0CTL1 & UCTXSTP);
    i2cSetupRx(i2cAddress);
    i2cReceive(data, numberOfBytes);
    while (UCB0STAT & UCBBUSY);
}
