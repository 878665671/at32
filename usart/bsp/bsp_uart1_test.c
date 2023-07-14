#include "bsp_uart1_test.h"
#include "device_usart.h"


void uart1_poll_model_test(void)
{
	char ch1[] = "uart1 poll model test.\n";
	char ch2[] = "uart1 poll ping.\nEnter a character for the command output.\n";
	uint16_t converted_ch1[sizeof(ch1) / sizeof(ch1[0]) * 2]; 
	for (int i = 0; i < sizeof(ch1) / sizeof(ch1[0]); i++) 
	{
			converted_ch1[i * 2] = ch1[i] & 0xFF; 
			converted_ch1[i * 2 + 1] = (ch1[i] >> 8) & 0xFF; 
	}
	uint16_t converted_ch2[sizeof(ch2) / sizeof(ch2[0]) * 2]; 
	for (int i = 0; i < sizeof(ch2) / sizeof(ch2[0]); i++) 
	{
			converted_ch2[i * 2] = ch2[i] & 0xFF; 
			converted_ch2[i * 2 + 1] = (ch2[i] >> 8) & 0xFF; 
	}
	uart_poll_init(115200);
	
	uart_transmit_poll(0,converted_ch1,sizeof(converted_ch1) / sizeof(converted_ch1[0]));
	uart_transmit_poll(0,converted_ch2,sizeof(converted_ch2) / sizeof(converted_ch2[0]));
	while(1)
	{
		uart_poll_ping();
	}
	

}

static void myCallbackFunction(uint16_t data)
{
	char ch[] = "uart1 is interrupted.\n";
	uint16_t converted[sizeof(ch) / sizeof(ch[0]) * 2]; 
	for (int i = 0; i < sizeof(ch) / sizeof(ch[0]); i++) 
	{
			converted[i * 2] = ch[i] & 0xFF; 
			converted[i * 2 + 1] = (ch[i] >> 8) & 0xFF; 
	}
	uart_send(converted, sizeof(converted) / sizeof(converted[0]));
}


void uart1_int_model_test(void)
{
	char ch1[] = "uart1 int model test.\n";
	char ch2[] = "Please enter a character to trigger an interrupt.\n";
	uint16_t converted_ch1[sizeof(ch1) / sizeof(ch1[0]) * 2]; 
	for (int i = 0; i < sizeof(ch1) / sizeof(ch1[0]); i++) 
	{
			converted_ch1[i * 2] = ch1[i] & 0xFF; 
			converted_ch1[i * 2 + 1] = (ch1[i] >> 8) & 0xFF; 
	}
	uint16_t converted_ch2[sizeof(ch2) / sizeof(ch2[0]) * 2]; 
	for (int i = 0; i < sizeof(ch2) / sizeof(ch2[0]); i++) 
	{
			converted_ch2[i * 2] = ch2[i] & 0xFF; 
			converted_ch2[i * 2 + 1] = (ch2[i] >> 8) & 0xFF; 
	}
	uart_int_init(115200);
	uart_send(converted_ch1, sizeof(converted_ch1) / sizeof(converted_ch1[0]));
	uart_send(converted_ch2, sizeof(converted_ch2) / sizeof(converted_ch2[0]));
	setCallback(myCallbackFunction);
}


void uart1_dma_model_test(void)
{
	char ch1[] = "uart1 dmapoll model test.\n";
	char ch2[] = "uart1 dmapoll ping.\nEnter a character for the command output.\nPlease enter 11 characters, such as hello, word!.\n";

	uart_dmaPoll_init(115200);
	uart_dmapoll_send((uint8_t *)&ch1, sizeof(ch1) / sizeof(ch1[0]));
	uart_dmapoll_send((uint8_t *)&ch2, sizeof(ch2) / sizeof(ch2[0]));

	while(1)
	{
	
		uart_dma_poll_ping();
	
	}
}

