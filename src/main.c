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
	GPIOB->DDR |= 0x01;
	//uint16_t length = sprintf(str,"MCUCR=%02x\r\n",MCUCR);
	//serialWriteBlocking(&Serial0, length, str);
//	serialWriteBlocking(&Serial0, 7, "Init\r\n");
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
		uint8_t data = ((GPIOD->PIN & 0x03)) | ((GPIOC->PIN & 0x30) >> 2);
		avrprintf(&LCD, "Data=%x", data);
		_delay_ms(10);
	}
	return 0;
}
