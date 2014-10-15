#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "GPIO.h"
#include "Serial.h"
#include "usbdrv.h"
#include "LCD.h"
#include "avrprintf.h"
#include <stdio.h>
char str[128]=__DATE__""__TIME__"\r\n";
//extern void updateUSB();


int main(){
//	serialInit(&Serial0, 9600);
//	usbInit();
	lcdInit();
//	lcdSetCursor(1,0);
/*	usbDeviceDisconnect();
	_delay_ms(100);
	usbDeviceConnect();*/
	sei();
	GPIOB->DDR |= (1<<0) | (1<<5);
	//GPIOB->DDR &= ~(1<<4);
	GPIOC->DDR |= (1<<5);
	GPIOC->PORT |= (1<<5);
	//uint16_t length = sprintf(str,"MCUCR=%02x\r\n",MCUCR);
	//serialWriteBlocking(&Serial0, length, str);
//	serialWriteBlocking(&Serial0, 7, "Init\r\n");
	SPCR |= (1 << SPE) | (1 << MSTR);
	uint8_t counter = 0;
	while(1){
		//uint16_t length = sprintf(str,"%d\r\n",counter++);
		//serialWrite(&Serial0, strlen(str), str);
		//serialWrite(&Serial0, length, str);
		//updateUSB();
		//_delay_ms(100);
		GPIOB->PIN |= 0x01;
		//lcdPrint("Hello World!");
		lcdClear();
		GPIOC->PORT &= ~(1<<5);// Parallel load
		asm volatile("nop\n\t");
		GPIOC->PORT |= (1<<5);
		asm volatile("nop\n\t");
		SPDR = 0x00;	// write dummy
		while((SPSR & (1<<SPIF)) == 0);
		uint16_t data = SPDR;
		SPDR = 0x00;	// write dummy
		while((SPSR & (1<<SPIF)) == 0);
		data = (data << 8) | SPDR;
		avrprintf(&LCD, "Data=%04x", data);
		_delay_ms(100);
	}
	return 0;
}
