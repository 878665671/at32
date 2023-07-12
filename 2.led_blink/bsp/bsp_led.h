#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "device_gpio.h"
struct my_gpio_manage
{
    uint8_t num;                  /* 已注册的数目 */
    gpio_struct *gpio_list[10]; /* 储存GPIO指针的数组 */
};

void led_int(void);
int gpio_register(void);
void led0_blink(void);
void key0_read(void);

#endif

