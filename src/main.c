#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Serial.h"
#include "usbdrv.h"
#include <stdio.h>
char str[128]=__DATE__""__TIME__"\r\n";
extern void updateUSB();


FUSEMEM __fuse_t fusedata = {
	0xFF,
#ifdef BOOTLOADER
	0xD8,		// BOOTRST = 0 (enabled)
#else
	0xD9,		// BOOTRST = 1 (disabled)
#endif
	0xFF
};


int main(){
#ifdef BOOTLOADER
	MCUCR = (1<<IVCE);	/*Move vector table to the Bootloader region.*/
	MCUCR = (1<<IVSEL);	/*Move vector table to the Bootloader region.*/
#endif
	serialInit(&Serial0, 9600);
	usbInit();
	usbDeviceDisconnect();
	_delay_ms(100);
	usbDeviceConnect();
	sei();
	//uint16_t length = sprintf(str,"MCUCR=%02x\r\n",MCUCR);
	//serialWriteBlocking(&Serial0, length, str);
	serialWriteBlocking(&Serial0, 7, "Init\r\n");
#ifdef BOOTLOADER
	serialWriteBlocking(&Serial0, 7, "Bootloader\r\n");
#endif
	uint16_t counter = 0;
	while(1){
		//uint16_t length = sprintf(str,"%d\r\n",counter++);
		serialWrite(&Serial0, strlen(str), str);
		//serialWrite(&Serial0, length, str);
		updateUSB();
		_delay_ms(20);
	}
	return 0;
}
