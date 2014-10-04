#define SIGRD 5
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include "isp_self.h"
#include "bootloader.h"

static uint8_t command, bytecount=0;
static uint8_t isp_data[4];
void ispConnect(){
	bytecount=0;
}

void ispDisconnect(){
	bytecount=0;
}

uint8_t ispTransmit(uint8_t send_byte){
	if(bytecount == 0){
		switch(send_byte){/*Decode command that should be used for ISP programming, now programming itself*/
		case 0x30:
			command = send_byte;
			bytecount = 1;
		}
	}
	else if(bytecount < 3){/*Read four bytes*/
		isp_data[bytecount++] = send_byte;
	}
	else{
		bytecount = 0;
		switch(command){
		case 0x30:
			return boot_signature_byte_get(isp_data[2]);
			ledOn();
		}
	}
}

uint8_t ispEnterProgrammingMode(){
	
	return 0;
}

uint8_t ispReadEEPROM(unsigned int address){
}

uint8_t ispWriteFlash(unsigned long address, uint8_t data, uint8_t pollmode){
}

uint8_t ispFlushPage(unsigned long address, uint8_t pollvalue){
}

uint8_t ispReadFlash(unsigned long address){
}

uint8_t ispWriteEEPROM(unsigned int address, uint8_t data){
}

void ispLoadExtendedAddressByte(unsigned long address){
}