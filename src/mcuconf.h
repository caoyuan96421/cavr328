
#ifndef __MCUCONF_H__
#define __MCUCONF_H__

/*Serial Config*/
#define MCUCONF_USE_SERIAL				1
#define MCUCONF_USE_SERIAL_ASYNC 		1
#define MCUCONF_SERIAL_TX_BUFFER_SIZE	64

/*LCD Driver Config*/
#define MCUCONF_USE_LCD					1
#define MCUCONF_LCD_USE_BUSY_CHECK		1
#define MCUCONF_LCD_E_PORT				GPIOB
#define MCUCONF_LCD_E_PIN				1
#define MCUCONF_LCD_RS_PORT				GPIOB
#define MCUCONF_LCD_RS_PIN				2
#define MCUCONF_LCD_RW_PORT				GPIOD
#define MCUCONF_LCD_RW_PIN				5
#define MCUCONF_LCD_DATA_PORT			GPIOC
#define MCUCONF_LCD_DATA_OFFSET			0

/*Printf utilites*/
#define MCUCONF_USE_PRINTF				1
#define MCUCONF_PRINTF_USE_FLOAT		1

#define MCUCONF_USE_USB					1

#endif /*__MCUCONF_H__*/