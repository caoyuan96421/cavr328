#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "isp_self.h"
#include "bootloader.h"

void ispConnect(){
}

void ispDisconnect(){
}

void ispProcessCommand(uint8_t command[], uint8_t reply[]){
	uint16_t i;
	reply[0]=reply[1]=reply[2]=reply[3]=0x00;
	switch(command[0]){
		case 0x30:
			if(command[2] == 0x01){ /*Pretend to be ATMega328P*/
				reply[3] = 0x95;
			}
			else if(command[2] == 0x02){
				reply[3] = 0x0F;
			}
			else{
				reply[3] = boot_signature_byte_get(command[2]);
			}
			break;
		case 0xAC:
			switch(command[1]){
				case 0x80:
					for(i=0;i<RWW_MAX_PAGE;i++){
						boot_page_erase_safe(i << PAGE_BITS);
					}
					boot_rww_enable_safe();
					break;
				default:
					break;
			}
			reply[1] = command[0];
			reply[2] = command[1];
			reply[3] = command[2]; /*Echo back*/
			break;
	}
}

uint8_t ispEnterProgrammingMode(){
	
	return 0;
}

static void memcpy_fill(uint8_t len, uint16_t buffer[]){
	asm volatile(
		"rjmp .memcpy_loop_start\n"
		".memcpy_loop:\n\t"
		"st X+,%0\n"
		".memcpy_loop_start:\n\t"
		"subi %1, 1\n\t"
		"brcc .memcpy_loop\n\t"
		: : "r" (FLASH_ERASED_BYTE), "r" (len), "x" (buffer)
	);
}

static void memcpy_prog(uint16_t address, uint8_t len, uint8_t buffer[]){
	asm volatile(
		"rjmp .memcpy_P_loop_start\n"
		".memcpy_P_loop:\n\t"
		"lpm __tmp_reg__, Z+\n\t"
		"st X+,__tmp_reg__\n"
		".memcpy_P_loop_start:\n\t"
		"subi %1, 1\n\t"
		"brcc .memcpy_P_loop\n\t"
	: : "z" ((uint16_t)address), "r" (len), "x" (buffer)
	);//Don't know why memcpy_P doesn't work. so i just copied its assembly
}

void ispReadFlash(uint16_t address, uint8_t len, uint8_t buffer[]){
	uint8_t i;
	if(address >= RWW_MAX_PAGE * PAGE_SIZE){
		memcpy_fill(len, buffer);
		//for(i=0;i<len;i++)
		//	buffer[i] = FLASH_ERASED_BYTE;
	}
	else{
		memcpy_prog(address, len, buffer);
		//for(i=0;i<len;i++)
		//	buffer[i] = pgm_read_byte(address + i);
	}
}

void ispWriteFlash(uint16_t address, uint8_t len, uint8_t buffer[]){
	uint8_t i;
	for(i=0;i<len;i+=2){
		cli();
		boot_page_fill(address+i,*((uint16_t *)(buffer+i)));
		sei();
		boot_spm_busy_wait();
		if(((address+i+2) & (PAGE_SIZE-1))== 0){ /* Page Boundary*/
			cli();
			boot_page_write(address+i+1); /* Write last page */
			sei();
			boot_spm_busy_wait();
			cli();
			boot_rww_enable();
			sei();
			boot_spm_busy();
		}
	}
}

void ispFlushPage(uint16_t address){
	cli();
	boot_page_write(address); /* Write page */
	sei();
	boot_spm_busy_wait();
	cli();
	boot_rww_enable();
	sei();
	boot_spm_busy_wait();
}

void ispWriteEEPROM(uint16_t address, uint8_t len, uint8_t buffer[]){
	eeprom_busy_wait();
	eeprom_write_block(buffer, (void *) address, len);
}

void ispReadEEPROM(uint16_t address, uint8_t len, uint8_t buffer[]){
	eeprom_busy_wait();
	eeprom_read_block(buffer, (void *) address, len);
}