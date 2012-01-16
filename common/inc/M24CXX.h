/* Author: Alan Barr
** Created: January 2012 
**
** Header file for M24C16, M24C08, M24C04, M24C02, M24C01 M24CXX.
** Register addresses and unique bits.
*/

#ifndef MC43CXX_H_
#define MC43CXX_H_

/* 7 bit address */
#define M24CXX_ADDRESS          0x50
#define M24CXX_ADDRESS_0        0x50
#define M24CXX_ADDRESS_1        0x51
#define M24CXX_ADDRESS_2        0x52
#define M24CXX_ADDRESS_3        0x53
#define M24CXX_ADDRESS_4        0x54
#define M24CXX_ADDRESS_5        0x55
#define M24CXX_ADDRESS_6        0x56
#define M24CXX_ADDRESS_7        0x57

/* 8 bit address */
#define M24CXX_8B_ADDRESS       0xA0
#define M24CXX_8B_ADDRESS_0     0xA0
#define M24CXX_8B_ADDRESS_1     0xA2
#define M24CXX_8B_ADDRESS_2     0xA4
#define M24CXX_8B_ADDRESS_3     0xA6
#define M24CXX_8B_ADDRESS_4     0xA8
#define M24CXX_8B_ADDRESS_5     0xAA
#define M24CXX_8B_ADDRESS_6     0xAC
#define M24CXX_8B_ADDRESS_7     0xAE

/* Address Pins */
#define M24CXX_ADDR_PIN0        0x01     /*A0*/
#define M24CXX_ADDR_PIN1        0x02     /*A1*/
#define M24CXX_ADDR_PIN2        0x04     /*A2*/

#define EEPROM_PAGE_SIZE        16

#endif /*MC43CXX_H_*/
