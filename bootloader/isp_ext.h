
#ifndef __ISP_EXT_H__
#define __ISP_EXT_H__

/* Implementation of ISP functions with self-programming*/

/* Prepare connection to target device */
void ispExtConnect();

/* Close connection to target device */
void ispExtDisconnect();

/* Process command from USB (which should have been forwarded to the target)*/
uint8_t ispExtTransmit(uint8_t data);/* Set SCK speed option*/void ispExtSetSCKOption(uint8_t speed);

/* enter programming mode */
uint8_t ispExtEnterProgrammingMode();

/* read byte from eeprom at given address */
void ispExtReadEEPROM(uint16_t address, uint8_t len, uint8_t buffer[]);

/* write byte to flash at given address */
void ispExtWriteFlash(uint16_t address, uint8_t len, uint8_t buffer[], uint16_t pagesize);

/* Force write page buffer to flash */
void ispExtFlushPage(uint16_t address);

/* read byte from flash at given address */
void ispExtReadFlash(uint16_t address, uint8_t len, uint8_t buffer[]);

/* write byte to eeprom at given address */
void ispExtWriteEEPROM(uint16_t address, uint8_t len, uint8_t buffer[]);


#endif /*__ISP_SELF_H__*/