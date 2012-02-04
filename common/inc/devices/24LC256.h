/* Author: Alan Barr
** Created: January 2012 
**
** Header file for 24AA256/24LC256/24FC256 EEPROM.
*/

#ifndef EEPROM_24LC256_H_
#define EEPROM_24LC256_H_

/* 7 bit address */
#define EEPROM_ADDRESS          0x50
#define EEPROM_ADDRESS_0        0x50
#define EEPROM_ADDRESS_1        0x51
#define EEPROM_ADDRESS_2        0x52
#define EEPROM_ADDRESS_3        0x53
#define EEPROM_ADDRESS_4        0x54
#define EEPROM_ADDRESS_5        0x55
#define EEPROM_ADDRESS_6        0x56
#define EEPROM_ADDRESS_7        0x57

/* 8 bit address */
#define EEPROM_8B_ADDRESS       0xA0
#define EEPROM_8B_ADDRESS_0     0xA0
#define EEPROM_8B_ADDRESS_1     0xA2
#define EEPROM_8B_ADDRESS_2     0xA4
#define EEPROM_8B_ADDRESS_3     0xA6
#define EEPROM_8B_ADDRESS_4     0xA8
#define EEPROM_8B_ADDRESS_5     0xAA
#define EEPROM_8B_ADDRESS_6     0xAC
#define EEPROM_8B_ADDRESS_7     0xAE

/* Address Pins */
#define EEPROM_ADDR_PIN0        0x01     /*A0*/
#define EEPROM_ADDR_PIN1        0x02     /*A1*/
#define EEPROM_ADDR_PIN2        0x04     /*A2*/

#define EEPROM_PAGE_SIZE        64

#endif /*EEPROM_24LC256_H_*/
