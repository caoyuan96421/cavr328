/* This source file deals with all USB related stuff
 * Author: Yuan Cao
 * Created on: 2014/09/30
 * File name: usb.c
*/

#include "usbdrv.h"
#include "bootloader.h"
#include "isp_self.h"

static uint8_t replyBuffer[8];

static uint8_t prog_state = PROG_STATE_IDLE;

static uint8_t prog_address_newmode = 0;
static unsigned long prog_address;
static unsigned int prog_nbytes = 0;
static unsigned int prog_pagesize;
static uint8_t prog_blockflags;
static uint8_t prog_pagecounter;

uint8_t usbFunctionSetup(uint8_t data[8]) {

	uint8_t len = 0;

	if (data[1] == USBASP_FUNC_CONNECT) {

		/* set compatibility mode of address delivering */
		prog_address_newmode = 0;

		ledOn();
		usart0_write("ISP connect\r\n");

	} else if (data[1] == USBASP_FUNC_DISCONNECT) {
		ledOff();
		usart0_write("ISP disconnect\r\n");
	} else if (data[1] == USBASP_FUNC_TRANSMIT) {
		replyBuffer[0] = ispTransmit(data[2]);
		replyBuffer[1] = ispTransmit(data[3]);
		replyBuffer[2] = ispTransmit(data[4]);
		replyBuffer[3] = ispTransmit(data[5]);
		len = 4;
		usart0_write("ISP transmit\r\n");

	} else if (data[1] == USBASP_FUNC_READFLASH) {

		if (!prog_address_newmode)
			prog_address = (data[3] << 8) | data[2];

		prog_nbytes = (data[7] << 8) | data[6];
		prog_state = PROG_STATE_READFLASH;
		len = 0xff; /* multiple in */
		usart0_write("ISP readflash\r\n");

	} else if (data[1] == USBASP_FUNC_READEEPROM) {

		if (!prog_address_newmode)
			prog_address = (data[3] << 8) | data[2];

		prog_nbytes = (data[7] << 8) | data[6];
		prog_state = PROG_STATE_READEEPROM;
		len = 0xff; /* multiple in */
		usart0_write("ISP readEEP\r\n");

	} else if (data[1] == USBASP_FUNC_ENABLEPROG) {
		replyBuffer[0] = ispEnterProgrammingMode();
		len = 1;
		usart0_write("ISP en prog\r\n");

	} else if (data[1] == USBASP_FUNC_WRITEFLASH) {

		if (!prog_address_newmode)
			prog_address = (data[3] << 8) | data[2];

		prog_pagesize = data[4];
		prog_blockflags = data[5] & 0x0F;
		prog_pagesize += (((unsigned int) data[5] & 0xF0) << 4);
		if (prog_blockflags & PROG_BLOCKFLAG_FIRST) {
			prog_pagecounter = prog_pagesize;
		}
		prog_nbytes = (data[7] << 8) | data[6];
		prog_state = PROG_STATE_WRITEFLASH;
		len = 0xff; /* multiple out */
		usart0_write("ISP writeflash\r\n");

	} else if (data[1] == USBASP_FUNC_WRITEEEPROM) {

		if (!prog_address_newmode)
			prog_address = (data[3] << 8) | data[2];

		prog_pagesize = 0;
		prog_blockflags = 0;
		prog_nbytes = (data[7] << 8) | data[6];
		prog_state = PROG_STATE_WRITEEEPROM;
		len = 0xff; /* multiple out */
		usart0_write("ISP writeeep\r\n");

	} else if (data[1] == USBASP_FUNC_SETLONGADDRESS) {

		/* set new mode of address delivering (ignore address delivered in commands) */
		prog_address_newmode = 1;
		/* set new address */
		prog_address = *((unsigned long*) &data[2]);
		usart0_write("ISP setladdr\r\n");

	} else if (data[1] == USBASP_FUNC_SETISPSCK) {

		/* set sck option */
		replyBuffer[0] = 0;
		len = 1;
		usart0_write("ISP setsck\r\n");

	} else if (data[1] == USBASP_FUNC_GETCAPABILITIES) {
		replyBuffer[0] = 0;	/*Doesn't support TPI capability*/
		replyBuffer[1] = 0;
		replyBuffer[2] = 0;
		replyBuffer[3] = 0;
		len = 4;
	}

	usbMsgPtr = replyBuffer;

	return len;
}

uint8_t usbFunctionRead(uint8_t *data, uint8_t len) {

	uint8_t i;

	/* check if programmer is in correct read state */
	if ((prog_state != PROG_STATE_READFLASH) && (prog_state
			!= PROG_STATE_READEEPROM)) {
		return 0xff;
	}

	/* fill packet ISP mode */
	for (i = 0; i < len; i++) {
		if (prog_state == PROG_STATE_READFLASH) {
			data[i] = ispReadFlash(prog_address);
		} else {
			data[i] = ispReadEEPROM(prog_address);
		}
		prog_address++;
	}

	/* last packet? */
	if (len < 8) {
		prog_state = PROG_STATE_IDLE;
	}

	return len;
}

uint8_t usbFunctionWrite(uint8_t *data, uint8_t len) {

	uint8_t retVal = 0;
	uint8_t i;

	/* check if programmer is in correct write state */
	if ((prog_state != PROG_STATE_WRITEFLASH) && (prog_state
			!= PROG_STATE_WRITEEEPROM)) {
		return 0xff;
	}

	for (i = 0; i < len; i++) {
		if (prog_state == PROG_STATE_WRITEFLASH) {
			/* Flash */
			if (prog_pagesize == 0) {
				/* not paged */
				ispWriteFlash(prog_address, data[i], 1);
			} else {
				/* paged */
				ispWriteFlash(prog_address, data[i], 0);
				prog_pagecounter--;
				if (prog_pagecounter == 0) {
					ispFlushPage(prog_address, data[i]);
					prog_pagecounter = prog_pagesize;
				}
			}
		} else {
			/* EEPROM */
			ispWriteEEPROM(prog_address, data[i]);
		}
		prog_nbytes--;
		if (prog_nbytes == 0) {
			prog_state = PROG_STATE_IDLE;
			if ((prog_blockflags & PROG_BLOCKFLAG_LAST) && (prog_pagecounter
					!= prog_pagesize)) {
				/* last block and page flush pending, so flush it now */
				ispFlushPage(prog_address, data[i]);
			}
			retVal = 1; // Need to return 1 when no more data is to be received
		}
		prog_address++;
	}
	return retVal;
}