/* Author: Alan Barr
** Created: January 2012 
**
** Header file for MCP23017 bit expander.
** Register addresses and unique bits.
*/

#ifndef MCP23017_H_
#define MCP23017_H_

/* Address pins */
#define MCP23017_ADDR_PIN0          0x01     /*A0*/
#define MCP23017_ADDR_PIN1          0x02     /*A1*/
#define MCP23017_ADDR_PIN2          0x04     /*A2*/

/* 7 Bit Addresses */
#define MCP23017_ADDRESS            0x20  
#define MCP23017_ADDRESS_0          0x20  
#define MCP23017_ADDRESS_1          0x21  
#define MCP23017_ADDRESS_2          0x22  
#define MCP23017_ADDRESS_3          0x23  
#define MCP23017_ADDRESS_4          0x24  
#define MCP23017_ADDRESS_5          0x25  
#define MCP23017_ADDRESS_6          0x26  
#define MCP23017_ADDRESS_7          0x27  

/* 8 Bit Addresses */
#define MCP23017_8B_ADDRESS         0x40     
#define MCP23017_8B_ADDRESS_0       0x40    
#define MCP23017_8B_ADDRESS_1       0x42    
#define MCP23017_8B_ADDRESS_2       0x44    
#define MCP23017_8B_ADDRESS_3       0x46    
#define MCP23017_8B_ADDRESS_4       0x48    
#define MCP23017_8B_ADDRESS_5       0x4A    
#define MCP23017_8B_ADDRESS_6       0x4C    
#define MCP23017_8B_ADDRESS_7       0x4E    

/* Register Addresses - MCP23017 */
#define MCP23017_IODIRA             0x00    /*I/O DIRECTION REGISTER A*/
#define MCP23017_IODIRB             0x01    /*I/O DIRECTION REGISTER B*/
#define MCP23017_IPOLA              0x02    /*INPUT POLARITY REGISTER A*/
#define MCP23017_IPOLB              0x03    /*INPUT POLARITY REGISTER B*/
#define MCP23017_GPINTENA           0x04    /*INTERRUPT-ON-CHANGE CONTROL REGISTER A*/
#define MCP23017_GPINTENB           0x05    /*INTERRUPT-ON-CHANGE CONTROL REGISTER B*/
#define MCP23017_DEFVALA            0x06    /*DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE A*/
#define MCP23017_DEFVALB            0x07    /*DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE B*/
#define MCP23017_INTCONA            0x08    /*INTERRUPT CONTROL REGISTER A*/
#define MCP23017_INTCONB            0x09    /*INTERRUPT CONTROL REGISTER B*/
#define MCP23017_IOCONA             0x0A    /*CONFIGURATION REGISTER A*/
#define MCP23017_IOCONB             0x0B    /*CONFIGURATION REGISTER B*/
#define MCP23017_GPPUA              0x0C    /*PULL-UP RESISTOR A*/
#define MCP23017_GPPUB              0x0D    /*PULL-UP RESISTOR B*/
#define MCP23017_INTFA              0x0E    /*INTERRUPT FLAG REGISTER A*/
#define MCP23017_INTFB              0x0F    /*INTERRUPT FLAG REGISTER B*/
#define MCP23017_INTCAPA            0x10    /*INTERRUPT CAPTURE REGISTER A*/
#define MCP23017_INTCAPB            0x11    /*INTERRUPT CAPTURE REGISTER B*/
#define MCP23017_GPIOA              0x12    /*PORT REGISTER A*/
#define MCP23017_GPIOB              0x13    /*PORT REGISTER B*/
#define MCP23017_OLATA              0x14    /*OUTPUT LATCH REGISTER  A*/
#define MCP23017_OLATB              0x15    /*OUTPUT LATCH REGISTER  B*/

#define MCP23017_IOCON              MCP23017_IOCONA 
/*Register addresses if BANK = 1 */
#define MCP23017_IODIRA_BANK        0x00    /*I/O DIRECTION REGISTER A*/                            
#define MCP23017_IODIRB_BANK        0x10    /*I/O DIRECTION REGISTER B*/
#define MCP23017_IPOLA_BANK         0x01    /*INPUT POLARITY REGISTER A*/
#define MCP23017_IPOLB_BANK         0x11    /*INPUT POLARITY REGISTER B*/
#define MCP23017_GPINTENA_BANK      0x02    /*INTERRUPT-ON-CHANGE CONTROL REGISTER A*/
#define MCP23017_GPINTENB_BANK      0x12    /*INTERRUPT-ON-CHANGE CONTROL REGISTER B*/
#define MCP23017_DEFVALA_BANK       0x03    /*DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE A*/
#define MCP23017_DEFVALB_BANK       0x13    /*DEFAULT COMPARE REGISTER FOR INTERRUPT-ON-CHANGE B*/
#define MCP23017_INTCONA_BANK       0x04    /*INTERRUPT CONTROL REGISTER A*/
#define MCP23017_INTCONB_BANK       0x14    /*INTERRUPT CONTROL REGISTER B*/
#define MCP23017_IOCONA_BANK        0x05    /*CONFIGURATION REGISTER A*/
#define MCP23017_IOCONB_BANK        0x15    /*CONFIGURATION REGISTER B*/
#define MCP23017_GPPUA_BANK         0x06    /*PULL-UP RESISTOR A*/
#define MCP23017_GPPUB_BANK         0x16    /*PULL-UP RESISTOR B*/
#define MCP23017_INTFA_BANK         0x07    /*INTERRUPT FLAG REGISTER A*/
#define MCP23017_INTFB_BANK         0x17    /*INTERRUPT FLAG REGISTER B*/
#define MCP23017_INTCAPA_BANK       0x08    /*INTERRUPT CAPTURE REGISTER A*/
#define MCP23017_INTCAPB_BANK       0x18    /*INTERRUPT CAPTURE REGISTER B*/
#define MCP23017_GPIOA_BANK         0x09    /*PORT REGISTER A*/
#define MCP23017_GPIOB_BANK         0x19    /*PORT REGISTER B*/
#define MCP23017_OLATA_BANK         0x0A    /*OUTPUT LATCH REGISTER A*/
#define MCP23017_OLATB_BANK         0x1A    /*OUTPUT LATCH REGISTER B*/

#define MCP23017_IOCON_BANK         MCP23017_IOCONA_BANK
/* IOCON bits */
#define MCP23017_INTPOL             0x20    /* This bit sets the polarity of the INT output pin.*/
#define MCP23017_ODR                0x40    /* This bit configures the INT pin as an open-drain output.*/
#define MCP23017_HAEN               0x80    /* Hardware Address Enable bit (MCP23S17 only).
                                            ** Address pins are always enabled on MCP23017.*/
#define MCP23017_DISSLW             0x10    /* Slew Rate control bit for SDA output.*/
#define MCP23017_SEQOP              0x20    /* Sequential Operation mode bit.*/
#define MCP23017_MIRROR             0x40    /* INT Pins Mirror bit.*/
#define MCP23017_BANK               0x80    /* Controls how the registers are addressed.*/

/* All other registers effect the associated pin. The following can be used to address these. */
#define MCP23017_PIN0               0x01
#define MCP23017_PIN1               0x02
#define MCP23017_PIN2               0x04
#define MCP23017_PIN3               0x08
#define MCP23017_PIN4               0x10
#define MCP23017_PIN5               0x20
#define MCP23017_PIN6               0x40
#define MCP23017_PIN7               0x80

#endif /*MCP23017_H_*/

