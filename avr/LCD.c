#include <avr/io.h>
#include <util/delay.h>
#include "GPIO.h"
#include "LCD.h"
#include "Channel.h"

#if MCUCONF_USE_LCD

#define setE \
	MCUCONF_LCD_E_PORT->PORT |= (1<<MCUCONF_LCD_E_PIN)
#define clrE \
	MCUCONF_LCD_E_PORT->PORT &= ~(1<<MCUCONF_LCD_E_PIN)
#define setCommand \
	MCUCONF_LCD_RS_PORT->PORT &= ~(1<<MCUCONF_LCD_RS_PIN)
#define setData \
	MCUCONF_LCD_RS_PORT->PORT |= (1<<MCUCONF_LCD_RS_PIN)
#define setWrite \
	MCUCONF_LCD_RW_PORT->PORT &= ~(1<<MCUCONF_LCD_RW_PIN)
#define setRead \
	MCUCONF_LCD_RW_PORT->PORT |= (1<<MCUCONF_LCD_RW_PIN)
	
static inline void _write_nibble(uint8_t nibble){
	MCUCONF_LCD_DATA_PORT->PORT &= ~(LCD_DATA_MASK);
	MCUCONF_LCD_DATA_PORT->PORT |= (nibble) << (MCUCONF_LCD_DATA_OFFSET);
	setE;
	_delay_us(LCD_DATA_DELAY);
	clrE;
}
static void _write_byte(uint8_t byte){
	/*High nibble*/
	MCUCONF_LCD_DATA_PORT->PORT &= ~(LCD_DATA_MASK);
	MCUCONF_LCD_DATA_PORT->PORT |= (byte & 0xF0) >> (4-MCUCONF_LCD_DATA_OFFSET);
	setE;
	_delay_us(LCD_DATA_DELAY);
	clrE;
	/*Low nibble*/
	MCUCONF_LCD_DATA_PORT->PORT &= ~(LCD_DATA_MASK);
	MCUCONF_LCD_DATA_PORT->PORT |= (byte & 0x0F) << MCUCONF_LCD_DATA_OFFSET;
	setE;
	_delay_us(LCD_DATA_DELAY);
	clrE;
}
static void _check_busy(){
	MCUCONF_LCD_DATA_PORT->DDR &= ~(LCD_DATA_MASK);
	setCommand;
	setRead;
	uint8_t busy = 1;
	while(busy){
		setE;
		_delay_us(LCD_DATA_DELAY);
		busy = MCUCONF_LCD_DATA_PORT->PIN & (1<<(MCUCONF_LCD_DATA_OFFSET + 3));
		clrE;
		setE;
		_delay_us(LCD_DATA_DELAY);
		clrE;
	}
	MCUCONF_LCD_DATA_PORT->DDR |= (LCD_DATA_MASK);
	setWrite;
}

void lcdInit(){
	/*Port init*/
	MCUCONF_LCD_E_PORT->DDR |= (1<<MCUCONF_LCD_E_PIN);
	MCUCONF_LCD_RS_PORT->DDR |= (1<<MCUCONF_LCD_RS_PIN);
	MCUCONF_LCD_RW_PORT->DDR |= (1<<MCUCONF_LCD_RW_PIN);
	MCUCONF_LCD_DATA_PORT->DDR |= LCD_DATA_MASK;
	clrE;
	setCommand;
	setWrite;
	_delay_ms(15);
	_write_nibble(0x3);
	_delay_ms(5);
	_write_nibble(0x3);
	_delay_ms(1);
	_write_nibble(0x3);
	_delay_ms(1);
	_write_nibble(0x2);
	_delay_ms(1);
	_write_byte(0x28);
	_check_busy();
	_write_byte(0x0C);
	_check_busy();
	_write_byte(0x06);
	_check_busy();
	_write_byte(0x01);
}

void lcdSendCommand(uint8_t command){
	_check_busy();
	setCommand;
	setWrite;
	_write_byte(command);
}

void lcdSendData(uint8_t data){
	_check_busy();
	setData;
	setWrite;
	_write_byte(data);
}

void lcdPrint(char *s){
	for(;*s;s++){
		_check_busy();
		setData;
		setWrite;
		_write_byte(*s);
	}
}

OutputChannel LCD = {
	(void (*)(char))lcdSendData,
};

#endif