
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

/* Processor Constants */
#define RWW_MAX_PAGE			224
#define PAGE_SIZE				128
#define PAGE_BITS				7
#define FLASH_ERASED_BYTE		0xFF

/* For pretending to be XW programmer */
#define XW_SELF_CHECK_BYTE		0xa7

/* USB function call identifiers */
#define USBASP_FUNC_CONNECT     1
#define USBASP_FUNC_DISCONNECT  2
#define USBASP_FUNC_TRANSMIT    3
#define USBASP_FUNC_READFLASH   4
#define USBASP_FUNC_ENABLEPROG  5
#define USBASP_FUNC_WRITEFLASH  6
#define USBASP_FUNC_READEEPROM  7
#define USBASP_FUNC_WRITEEEPROM 8
#define USBASP_FUNC_SETLONGADDRESS 9
#define USBASP_FUNC_SETISPSCK 10
#define USBASP_FUNC_GETCAPABILITIES 127
#define USBASP_FUNC_XW_SELFCHECK	0x69

/* USBASP capabilities */
#define USBASP_CAP_0_TPI    0x01

/* programming state */
#define PROG_STATE_IDLE         0
#define PROG_STATE_WRITEFLASH   1
#define PROG_STATE_READFLASH    2
#define PROG_STATE_READEEPROM   3
#define PROG_STATE_WRITEEEPROM  4

/* Block mode flags */
#define PROG_BLOCKFLAG_FIRST    1
#define PROG_BLOCKFLAG_LAST     2

/* macros for gpio functions */
#define ledOn()    PORTB |= 0x01
#define ledOff()   PORTB &= ~0x01

/*void usart0_write(const char *s);
void usart0_write_hex(uint8_t data);
void usart0_write_hex_word(uint16_t data);*/

#endif /*__BOOTLOADER_H__*/