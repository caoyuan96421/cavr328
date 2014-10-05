
#ifndef __ISP_SELF_H__
#define __ISP_SELF_H__

/* Implementation of ISP functions with self-programming*/

/* Prepare connection to target device */
void ispConnect();

/* Close connection to target device */
void ispDisconnect();

/* Process command from USB (which should have been forwarded to the target)*/
void ispProcessCommand(uint8_t command[], uint8_t reply[]);

/* enter programming mode */
uint8_t ispEnterProgrammingMode();

/* read byte from eeprom at given address */
void ispReadEEPROM(unsigned int address, uint8_t len, uint8_t buffer[]);

/* write byte to flash at given address */
void ispWriteFlash(unsigned long address, uint8_t len, uint8_t buffer[]);

/* Force write page buffer to flash */
void ispFlushPage(unsigned long address);

/* read byte from flash at given address */
void ispReadFlash(uint16_t address, uint8_t len, uint8_t buffer[]);

/* write byte to eeprom at given address */
void ispWriteEEPROM(unsigned int address, uint8_t len, uint8_t buffer[]);


#endif /*__ISP_SELF_H__*/