#include "device_gpio.h"
#include "bsp_led.h"
static struct my_gpio_manage led_manage = {0};
static struct my_gpio_manage key_manage = {0};
static gpio_struct led0 = {0};
static gpio_struct key0 = {0};
void led_int(void)
{
	led0.gpiox = GPIOE;
	led0.gpio_crm_clk = CRM_GPIOE_PERIPH_CLOCK;
	led0.pinx = GPIO_PINS_15;
	led0.gpio_mode = GPIO_MODE_OUTPUT;
	led_manage.gpio_list[led_manage.num] = &led0;
	printf("led_manage num = %d\n",led_manage.num + 1);
	key0.gpiox = GPIOB;
	key0.gpio_crm_clk = CRM_GPIOB_PERIPH_CLOCK;
	key0.pinx = GPIO_PINS_9;
	key0.gpio_mode = GPIO_MODE_INPUT;
	key_manage.gpio_list[key_manage.num] = &key0;
	printf("key_manage num = %d\n",key_manage.num + 1);
}


int gpio_register(void)
{
	for(uint8_t i = 0;i <= led_manage.num;i++)
		at32_gpio_init(led_manage.gpio_list[i]);
	
	for(uint8_t i = 0;i <= key_manage.num;i++)
		at32_gpio_init(key_manage.gpio_list[i]);
	return 0;
}


void led0_blink(void)
{
	gpio_write(led_manage.gpio_list[0],0);
	delay_ms(500);
	gpio_write(led_manage.gpio_list[0],1);
	delay_ms(500);
}


void key0_read(void)
{
	static uint8_t state;
	state = gpio_read(key_manage.gpio_list[0]);
	printf("key0 state = %d\n",state);

}
