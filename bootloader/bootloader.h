
#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

/*Hardware Connections*/
#define ISP_GPIO GPIOB
#define ISP_SCK 5
#define ISP_MOSI 3
#define ISP_MISO 4
#define RST_GPIO GPIOD
#define RST_PIN 4

/* Processor Constants for self-programming*/
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
#define USBASP_CAP_0_TPI    0x00

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

/* ISP SCK speed identifiers */
#define USBASP_ISP_SCK_AUTO   0
#define USBASP_ISP_SCK_0_5    1   /* 500 Hz */
#define USBASP_ISP_SCK_1      2   /*   1 kHz */
#define USBASP_ISP_SCK_2      3   /*   2 kHz */
#define USBASP_ISP_SCK_4      4   /*   4 kHz */
#define USBASP_ISP_SCK_8      5   /*   8 kHz */
#define USBASP_ISP_SCK_16     6   /*  16 kHz */
#define USBASP_ISP_SCK_32     7   /*  32 kHz */
#define USBASP_ISP_SCK_93_75  8   /*  93.75 kHz */
#define USBASP_ISP_SCK_187_5  9   /* 187.5  kHz */
#define USBASP_ISP_SCK_375    10  /* 375 kHz   */
#define USBASP_ISP_SCK_750    11  /* 750 kHz   */
#define USBASP_ISP_SCK_1500   12  /* 1.5 MHz   */#define USBASP_ISP_SCK_2000   13  /* 2.0 MHz   */

void usart0_write(const char *s);
void usart0_write_hex(uint8_t data);
void usart0_write_hex_word(uint16_t data);

#endif /*__BOOTLOADER_H__*/