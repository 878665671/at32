#ifndef __DEVICE_USART_H
#define __DEVICE_USART_H
#include "at32f435_437_board.h"

#define PRINT_UART                       USART1
#define PRINT_UART_CRM_CLK               CRM_USART1_PERIPH_CLOCK
#define PRINT_UART_TX_PIN                GPIO_PINS_9
#define PRINT_UART_TX_GPIO               GPIOA
#define PRINT_UART_TX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK
#define PRINT_UART_TX_PIN_SOURCE         GPIO_PINS_SOURCE9
#define PRINT_UART_TX_PIN_MUX_NUM        GPIO_MUX_7

#define PRINT_UART_RX_PIN                GPIO_PINS_10
#define PRINT_UART_RX_GPIO               GPIOA
#define PRINT_UART_RX_GPIO_CRM_CLK       CRM_GPIOA_PERIPH_CLOCK
#define PRINT_UART_RX_PIN_SOURCE         GPIO_PINS_SOURCE10
#define PRINT_UART_RX_PIN_MUX_NUM        GPIO_MUX_7
#define USARTx_IRQn										   USART1_IRQn

#define PRINT_UART_DMA_CRM_CLK 					 CRM_DMA1_PERIPH_CLOCK
#define DMAx_Port 					   					 DMA1
#define UART_DMA_TX_CH				 					 DMA1_CHANNEL1
#define DMAMUX_TX_CH				 						 DMA1MUX_CHANNEL1
#define DMAMUX_CH_TO_TXBASE			 				 DMAMUX_DMAREQ_ID_USART1_TX
#define UART_DMA_RX_CH				 					 DMA1_CHANNEL2
#define DMAMUX_RX_CH				 						 DMA1MUX_CHANNEL2
#define DMAMUX_CH_TO_RXBASE			 				 DMAMUX_DMAREQ_ID_USART1_RX

typedef void (*CallbackFunction)(uint16_t data);

void uart_poll_init(uint32_t baudrate);
void uart_poll_ping(void);
void uart_transmit_poll(uint8_t cmd,uint16_t *ch,uint8_t length);

void uart_int_init(uint32_t baudrate);
void uart_send(uint16_t *ch,uint8_t length);
void setCallback(CallbackFunction cb);

void uart_dmaPoll_init(uint32_t baudrate);
void uart_dmapoll_send(uint8_t *ch,uint16_t length);
void uart_dmapoll_read(uint8_t *ch,uint16_t length);
void uart_dma_poll_ping(void);

#endif