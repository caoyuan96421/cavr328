/* This source file deals with all USB related stuff
 * Author: Yuan Cao
 * Created on: 2014/09/30
 * File name: usb.c
*/

#include "usbdrv.h"
#include "bootloader.h"
#include "isp_self.h"
#include "isp_ext.h"
#include <util/delay.h>

static uint8_t replyBuffer[8];

static uint8_t prog_state = PROG_STATE_IDLE;

static uint8_t prog_address_newmode = 0;
static uint8_t prog_sck = USBASP_ISP_SCK_AUTO;
static uint16_t prog_address;
static uint16_t prog_nbytes = 0;
static uint16_t prog_pagesize;
static uint8_t prog_blockflags;
extern uint8_t self_program;

uint8_t usbFunctionSetup(uint8_t data[8]) {

	uint8_t len = 0, data_op = 0;
	if (data[1] == USBASP_FUNC_XW_SELFCHECK){
		replyBuffer[0] = ~(data[2]^data[3]^data[4]^XW_SELF_CHECK_BYTE);
		len = 1;
	}
	else if (data[1] == USBASP_FUNC_CONNECT) {
		
		/* set compatibility mode of address delivering */
		prog_address_newmode = 0;
		
		if(!self_program){
			switch(data[2]){
			case 1:
				prog_sck = USBASP_ISP_SCK_187_5;
				break;
			case 2:
				prog_sck = USBASP_ISP_SCK_375;
				break;
			case 3:
				prog_sck = USBASP_ISP_SCK_750;
				break;
			case 4:
				prog_sck = USBASP_ISP_SCK_1500;
				break;
			case 5:
				prog_sck = USBASP_ISP_SCK_2000;
				break;
			default:
				break;
			}
			ispExtSetSCKOption(prog_sck);
			ispExtConnect();
		}

	} else if (data[1] == USBASP_FUNC_DISCONNECT) {
		if(!self_program){
			ispExtDisconnect();
		}
	} else if (data[1] == USBASP_FUNC_TRANSMIT) {
		if(self_program){
			ispProcessCommand(data+2, replyBuffer);
		}
		else{
			replyBuffer[0] = ispExtTransmit(data[2]);
			replyBuffer[1] = ispExtTransmit(data[3]);
			replyBuffer[2] = ispExtTransmit(data[4]);
			replyBuffer[3] = ispExtTransmit(data[5]);
		}
		len = 4;
	} else if (data[1] == USBASP_FUNC_ENABLEPROG) {
		replyBuffer[0] = (self_program ? ispEnterProgrammingMode() : ispExtEnterProgrammingMode());
		len = 1;

	} else if (data[1] == USBASP_FUNC_READFLASH) {
		prog_state = PROG_STATE_READFLASH;
		data_op = 1;
		len = 0xff; /* multiple in */

	} else if (data[1] == USBASP_FUNC_READEEPROM) {
	
		prog_state = PROG_STATE_READEEPROM;
		data_op = 1;
		len = 0xff; /* multiple in */
	} else if (data[1] == USBASP_FUNC_WRITEFLASH) {

		prog_pagesize = data[4];
		prog_blockflags = data[5] & 0x0F;
		prog_pagesize += (((unsigned int) data[5] & 0xF0) << 4);
		prog_state = PROG_STATE_WRITEFLASH;
		data_op = 1;
		if(self_program && prog_pagesize != PAGE_SIZE){
			len = 0;
		}
		else{
			len = 0xff; /* multiple out */
		}
	} else if (data[1] == USBASP_FUNC_WRITEEEPROM) {
		prog_pagesize = 0;
		prog_blockflags = 0;
		prog_state = PROG_STATE_WRITEEEPROM;
		data_op = 1;
		len = 0xff; /* multiple out */

	} else if (data[1] == USBASP_FUNC_SETLONGADDRESS) {

		/* set new mode of address delivering (ignore address delivered in commands) */
		prog_address_newmode = 1;
		/* set new address */
		prog_address = *((uint16_t*) &data[2]);

	} else if (data[1] == USBASP_FUNC_SETISPSCK) {
		/* set sck option */
		if(!self_program){
			prog_sck = data[2];
		}
		replyBuffer[0] = 0;
		len = 1;

	} else if (data[1] == USBASP_FUNC_GETCAPABILITIES) {
		*((uint32_t *)replyBuffer) = 0;	/*Doesn't support TPI capability*/
		len = 4;
	}
	
	if(data_op){
		/*Common for Read/Write Flash/EEPROM*/
		if (!prog_address_newmode)
			prog_address = (data[3] << 8) | data[2];
		
		prog_nbytes = (data[7] << 8) | data[6];
	}
	
	usbMsgPtr = replyBuffer;

	return len;
}

uint8_t usbFunctionRead(uint8_t *data, uint8_t len) {

	/* check if programmer is in correct read state */
	if ((prog_state != PROG_STATE_READFLASH) && (prog_state
			!= PROG_STATE_READEEPROM)) {
		return 0xff;
	}

	/* fill packet ISP mode */
	if (prog_state == PROG_STATE_READFLASH) {
		if(self_program)
			ispReadFlash(prog_address, len, data);
		else{
			ispExtReadFlash(prog_address, len, data);
		}
	} else {
		if(self_program)
			ispReadEEPROM(prog_address, len, data);
		else{
			ispExtReadEEPROM(prog_address, len, data);
		}
	}
	prog_address += len;

	/* last packet? */
	if (len < 8) {
		prog_state = PROG_STATE_IDLE;
	}

	return len;
}

uint8_t usbFunctionWrite(uint8_t *data, uint8_t len) {
	/* check if programmer is in correct write state */
	if ((prog_state != PROG_STATE_WRITEFLASH) && (prog_state
			!= PROG_STATE_WRITEEEPROM)) {
		return 0xff;
	}
	
	if(prog_state == PROG_STATE_WRITEFLASH){
		if(self_program){
			if(prog_address < PAGE_SIZE * RWW_MAX_PAGE) /*Make sure we're writing in RWW region*/
				ispWriteFlash(prog_address, len, data);
		}
		else
			ispExtWriteFlash(prog_address, len, data, prog_pagesize);
		prog_address += len;
		prog_nbytes -= len;
		if(prog_nbytes <= 0){
			if((prog_blockflags & PROG_BLOCKFLAG_LAST) && (prog_address & (prog_pagesize-1)) != 0){ /* If not aligned on page boundary */
				if(self_program)
					ispFlushPage(prog_address);
				else
					ispExtFlushPage(prog_address);
			}
			prog_state = PROG_STATE_IDLE;
			return 1;		/* No more data to receive */
		}
	}
	else{
		/*Write EEPROM*/
		if(self_program)
			ispWriteEEPROM(prog_address, len, data);
		else
			ispExtWriteEEPROM(prog_address, len, data);
		prog_address += len;
		prog_nbytes -= len;
		if(prog_nbytes == 0){
			prog_state = PROG_STATE_IDLE;
			return 1;
		}
	}
	return 0;
}