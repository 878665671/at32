
#include "device_gpio.h"
uint32_t exint_line;

void at32_gpio_init(gpio_struct *gpio)
{
	gpio_init_type gpio_init_struct;						//gpio �ṹ��
 
	/* enable the led clock */
	crm_periph_clock_enable(gpio->gpio_crm_clk, TRUE);  //����gpiod��ʱ��
 
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);				//����gpio�ṹ��Ĭ��ֵ
 
	/* configure the led gpio */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER; 	//�ϴ�����ƶ�/��������
	gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;		 		//������߿�©������ģʽʱ�����������Ч��
	gpio_init_struct.gpio_mode = gpio->gpio_mode;		 					//���ģʽ
	gpio_init_struct.gpio_pins = gpio->pinx;	//IO �ܽ�
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE; 							//��������
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

