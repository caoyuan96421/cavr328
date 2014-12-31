/* This source file deals with all USB related stuff
 * Author: Yuan Cao
 * Created on: 2014/09/30
 * File name: usb.c
*/
#include "mcuconf.h"
#include "usbdrv.h"
#include "GPIO.h"

#if MCUCONF_USE_USB

#define USB_READ_DATA	0x01
#define USB_WRITE_DATA	0x02

extern uint16_t out_data;

USB_PUBLIC usbMsgLen_t usbFunctionSetup(uchar data[8]){
	usbRequest_t    *rq = (usbRequest_t *)data;

    /* The following requests are never used. But since they are required by
     * the specification, we implement them in this example.
     */
	 
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_VENDOR){    /* class request type */
        //DBG1(0x50, &rq->bRequest, 1);   /* debug output: print our request */
        if(rq->bRequest == USB_READ_DATA){ 
			//usbMsgPtr = &in_data;
			//GPIOB->PORT |= 0x01;
			return 2;
        }else if(rq->bRequest == USB_WRITE_DATA){
			out_data = *((uint16_t *)(data+2));
			GPIOB->PORT |= 0x01;
			return 0;
        }
    }
    return 0;   /* default for not implemented requests: return no data back to host */
}

#endif