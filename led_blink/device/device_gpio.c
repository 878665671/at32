
#include "device_gpio.h"
uint32_t exint_line;

void at32_gpio_init(gpio_struct *gpio)
{
	gpio_init_type gpio_init_struct;						//gpio 结构体
 
	/* enable the led clock */
	crm_periph_clock_enable(gpio->gpio_crm_clk, TRUE);  //开启gpiod的时钟
 
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);				//设置gpio结构体默认值
 
	/* configure the led gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER; 	//较大电流推动/吸入能力
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;		 		//推挽或者开漏（输入模式时，这个参数无效）
	gpio_init_struct.gpio_mode = gpio->gpio_mode;		 					//输出模式
	gpio_init_struct.gpio_pins = gpio->pinx;	//IO 管脚
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE; 							//无上下拉
	gpio_init(gpio->gpiox, &gpio_init_struct);
	gpio_bits_write(gpio->gpiox,gpio->pinx,gpio->init_val); 

	
	
}

uint8_t gpio_write(gpio_struct *gpio,uint8_t val)
{
	gpio_bits_write(gpio->gpiox,gpio->pinx,val); 
	return gpio->state = gpio_output_data_bit_read(gpio->gpiox,gpio->pinx);
}

uint8_t gpio_read(gpio_struct *gpio)
{ 
	gpio_input_data_bit_read(gpio->gpiox,gpio->pinx);
	delay_ms(10);
	return gpio->state = gpio_input_data_bit_read(gpio->gpiox,gpio->pinx);
}

