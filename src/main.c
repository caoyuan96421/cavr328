#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Serial.h"
#include "usbdrv.h"
const char str[] = "Hello World!1234\r\n"__DATE__""__TIME__;
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
	MCUCR |= (1<<IVCE);	/*Move vector table to the Bootloader region.*/
	MCUCR |= (1<<IVSEL);	/*Move vector table to the Bootloader region.*/
#endif
	serialInit(&Serial0, 9600);
	//usbInit();
	//usbDeviceDisconnect();
	//_delay_ms(100);
	//usbDeviceConnect();
	sei();
	while(1){
		//serialWriteBlocking(&Serial0, sizeof(str), str);
		serialWrite(&Serial0, sizeof(str), str);
		//updateUSB();
		_delay_ms(100);
	}
	return 0;
}
