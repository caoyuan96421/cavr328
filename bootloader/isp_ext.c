#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "isp_ext.h"
#include "bootloader.h"
#include "GPIO.h"

#define spiHWdisable() SPCR = 0

static uint8_t sck_spcr;
static uint8_t sck_spsr;

static void spiHWenable(){
	SPCR = sck_spcr;
	SPSR = sck_spsr;
}

static void ispdelay(){
	_delay_us(320);
}

void ispExtSetSCKOption(uint8_t option){
	if(option == USBASP_ISP_SCK_AUTO)
		option = USBASP_ISP_SCK_375;
	if(option <= USBASP_ISP_SCK_187_5){
		/* enable SPI, master, XTAL/128 (125kHz) */
		sck_spcr = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 >> SPR0);
		sck_spsr = 0;
	}
	else if(option <= USBASP_ISP_SCK_375){
		/* enable SPI, master, XTAL/64 (250kHz) */
		sck_spcr = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 >> SPR0);
		sck_spsr = (1 << SPI2X);
	}
	else if(option <= USBASP_ISP_SCK_750){
		/* enable SPI, master, XTAL/32 (500kHz) */
		sck_spcr = (1 << SPE) | (1 << MSTR) | (1 << SPR1);
		sck_spsr = (1 << SPI2X);
	}
	else if(option <= USBASP_ISP_SCK_1500){
		/* enable SPI, master, XTAL/16 (1000kHz) */
		sck_spcr = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
		sck_spsr = 0;
	}
	else{
		/* enable SPI, master, XTAL/8 (2000kHz) */
		sck_spcr = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
		sck_spsr = (1 << SPI2X);
	}
	//lcdSendData(level+48);
}

void ispExtConnect(){
	ISP_GPIO->DDR |= (1 << ISP_SCK) | (1 << ISP_MOSI);
	ISP_GPIO->DDR &= ~(1 << ISP_MISO);
	RST_GPIO->DDR |= (1 << RST_PIN);
	/*Reset Device*/
	RST_GPIO->PORT &= ~(1 << RST_PIN);
	ISP_GPIO->PORT &= ~(1 << ISP_SCK);
	
	ispdelay();
	RST_GPIO->PORT |= (1 << RST_PIN);
	ispdelay();
	RST_GPIO->PORT &= ~(1 << RST_PIN);
	ispdelay();
	
	spiHWenable();
}

void ispExtDisconnect(){
	ISP_GPIO->DDR &= ~((1 << ISP_SCK) | (1 << ISP_MOSI));
	RST_GPIO->DDR &= ~(1 << RST_PIN);
	ISP_GPIO->PORT &= ~((1 << ISP_SCK) | (1 << ISP_MOSI));
	RST_GPIO->PORT &= ~(1 << RST_PIN);
	
	spiHWdisable();
}

uint8_t ispExtTransmit(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1 << SPIF)));
	return SPDR;
}

uint8_t ispExtEnterProgrammingMode(){
	uint8_t check;
	uint8_t count = 32;
	while(count --){
		ispExtTransmit(0xAC);
		ispExtTransmit(0x53);
		check = ispExtTransmit(0x00);
		ispExtTransmit(0x00);
		
		if(check == 0x53){
			return 0;
		}
		
		spiHWdisable();
		ispdelay();
		RST_GPIO->PORT |= (1 << RST_PIN);
		ispdelay();
		RST_GPIO->PORT &= ~(1 << RST_PIN);
		ispdelay();
		
		spiHWenable();
	}
	return 1;	/*Failed*/
}

void ispExtReadFlash(uint16_t address, uint8_t len, uint8_t buffer[]){
	uint8_t i;
	for(i=0;i<len;i++,address++){
		ispExtTransmit(0x20 | ((address & 1) << 3));
		ispExtTransmit(address >> 9);
		ispExtTransmit(address >> 1);
		buffer[i] = ispExtTransmit(0x00);
	}
}

void ispExtWriteFlash(uint16_t address, uint8_t len, uint8_t buffer[], uint16_t pagesize){
	uint8_t i;
	for(i=0;i<len;i++,address++){
		ispExtTransmit(0x40 | ((address & 1) << 3));
		ispExtTransmit(address >> 9);
		ispExtTransmit(address >> 1);
		ispExtTransmit(buffer[i]);
		if(((address+1) & (pagesize-1)) == 0){//Page Boundary
			ispExtFlushPage(address);
		}
	}
}

void ispExtFlushPage(uint16_t address){
	ispExtTransmit(0x4C);
	ispExtTransmit(address >> 9);
	ispExtTransmit(address >> 1);
	ispExtTransmit(0x00);
	_delay_ms(5);
}

void ispExtWriteEEPROM(uint16_t address, uint8_t len, uint8_t buffer[]){
	uint8_t i;
	for(i=0;i<len;i++,address++){
		ispExtTransmit(0xC0);
		ispExtTransmit(address >> 8);
		ispExtTransmit(address);
		ispExtTransmit(buffer[i]);
		_delay_ms(5);
	}
}

void ispExtReadEEPROM(uint16_t address, uint8_t len, uint8_t buffer[]){
	uint8_t i;
	for(i=0;i<len;i++,address++){
		ispExtTransmit(0xA0);
		ispExtTransmit(address >> 8);
		ispExtTransmit(address);
		buffer[i] = ispExtTransmit(0x00);
	}
}