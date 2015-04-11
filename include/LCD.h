
#ifndef __LCD_H__
#define __LCD_H__

#include "mcuconf.h"
#include "avrprintf.h"
#if MCUCONF_USE_LCD

#if !defined(MCUCONF_LCD_E_PORT) || !defined(MCUCONF_LCD_E_PIN) || !defined(MCUCONF_LCD_RS_PORT)\
|| !defined(MCUCONF_LCD_RS_PIN) || !defined(MCUCONF_LCD_RW_PORT) || !defined(MCUCONF_LCD_RW_PIN)\
|| !defined(MCUCONF_LCD_DATA_PORT) || !defined(MCUCONF_LCD_DATA_OFFSET)
#error "MCUCONF_LCD: Port and pin definition not complete. Please check your mcuconf.h."
#endif

#define LCD_DATA_MASK	(0x0F << MCUCONF_LCD_DATA_OFFSET)
#define LCD_DATA_DELAY	1	/*us*/

/*Macro functions*/
#if !MCUCONF_LCD_USE_BUSY_CHECK
	#define lcdHome()				{lcdSendCommand(0x02);_delay_ms(1.6);}
	#define lcdClear()				{lcdSendCommand(0x01);_delay_ms(1.6);}
#else
	#define lcdHome()				lcdSendCommand(0x02)
	#define lcdClear()				lcdSendCommand(0x01)
#endif

#define lcdDisplayOff()			lcdSendCommand(0x08)

#define lcdSetCursor(on, blink) lcdSendCommand(0x0C | (on ? 0x02 : 0) | (blink ? 0x01:0))

#define lcdSetPosition(row, col) lcdSendCommand(0x80 | (row ? 0x40 : 0) | col);

/*dir=1: shift right. dir=0: shift left*/
#define lcdShift(dir)			lcdSendCommand(0x10 | (dir ? 0x0C : 0x08))
#define lcdShiftCursor(dir)		lcdSendCommand(0x10 | (dir ? 0x04 : 0x00))

/*Basic API*/
void lcdInit();
void lcdSendCommand(uint8_t command);
void lcdSendData(uint8_t data);

/*Advanced functions*/
void lcdPrint(char *s);

extern OutputChannel LCD;

#endif

#endif /*__LCD_H__*/