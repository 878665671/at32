
#ifndef __DEVICE_GPIO_H
#define __DEVICE_GPIO_H
#include "at32f435_437_board.h"
//typedef void (*my_gpio_callback)(void*);

typedef struct my_gpio
{
	crm_periph_clock_type gpio_crm_clk;
	gpio_type *gpiox;
	uint16_t pinx;
	uint8_t gpio_mode;
	uint8_t init_val;
	uint8_t state;
	
//	scfg_port_source_type gpiox_int_port;
//	scfg_pins_source_type int_pinx;
//	uint32_t int_line_x;
//	exint_polarity_config_type int_type;
//	my_gpio_callback callback;
	
}gpio_struct;


void at32_gpio_init(gpio_struct *gpio);
uint8_t gpio_write(gpio_struct *gpio,uint8_t val);
uint8_t gpio_read(gpio_struct *gpio);



#endif