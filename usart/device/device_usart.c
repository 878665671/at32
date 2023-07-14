#include "device_usart.h"

CallbackFunction temp_callback;

void uart_poll_init(uint32_t baudrate)
{
	/* Configure gpio first */
  gpio_init_type gpio_init_struct; 
	
  /* enable the uart and gpio clock */
  crm_periph_clock_enable(PRINT_UART_CRM_CLK, TRUE);
  crm_periph_clock_enable(PRINT_UART_TX_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(PRINT_UART_RX_GPIO_CRM_CLK,TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the uart tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = PRINT_UART_TX_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(PRINT_UART_TX_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = PRINT_UART_RX_PIN;
	gpio_init(PRINT_UART_RX_GPIO, &gpio_init_struct);

  gpio_pin_mux_config(PRINT_UART_TX_GPIO, PRINT_UART_TX_PIN_SOURCE, PRINT_UART_TX_PIN_MUX_NUM);
  gpio_pin_mux_config(PRINT_UART_RX_GPIO, PRINT_UART_RX_PIN_SOURCE, PRINT_UART_RX_PIN_MUX_NUM);

  /* configure uart param */
  usart_init(PRINT_UART, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
	usart_parity_selection_config(PRINT_UART, USART_PARITY_NONE);
  usart_transmitter_enable(PRINT_UART, TRUE);
	usart_receiver_enable(PRINT_UART, TRUE);
  usart_enable(PRINT_UART, TRUE);
}

void uart_poll_ping(void)
{
	uint16_t ch;
	while(usart_flag_get(PRINT_UART, USART_RDBF_FLAG) == RESET);
	ch = usart_data_receive(PRINT_UART);
	while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET);
	usart_data_transmit(PRINT_UART, ch);
}

void uart_transmit_poll(uint8_t cmd,uint16_t *ch,uint8_t length)
{
	if(cmd == 0)
	{
		for(uint8_t i = 0;i < length;i++)
		{
			while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET){};
			usart_data_transmit(PRINT_UART, *ch);
			ch++;
		}
	}
	else if(cmd == 1)
	{
	
		for(uint8_t i = 0;i < length;i++)
		{
			while(usart_flag_get(PRINT_UART, USART_RDBF_FLAG) == RESET);
			*ch = usart_data_receive(PRINT_UART);
			ch++;
		}
	}
}


void uart_int_init(uint32_t baudrate)
{
		/* Configure gpio first */
  gpio_init_type gpio_init_struct; 
	
  /* enable the uart and gpio clock */
  crm_periph_clock_enable(PRINT_UART_CRM_CLK, TRUE);
  crm_periph_clock_enable(PRINT_UART_TX_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(PRINT_UART_RX_GPIO_CRM_CLK,TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the uart tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = PRINT_UART_TX_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(PRINT_UART_TX_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = PRINT_UART_RX_PIN;
	gpio_init(PRINT_UART_RX_GPIO, &gpio_init_struct);

  gpio_pin_mux_config(PRINT_UART_TX_GPIO, PRINT_UART_TX_PIN_SOURCE, PRINT_UART_TX_PIN_MUX_NUM);
  gpio_pin_mux_config(PRINT_UART_RX_GPIO, PRINT_UART_RX_PIN_SOURCE, PRINT_UART_RX_PIN_MUX_NUM);

	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(USARTx_IRQn, 0, 0);

  /* configure uart param */
  usart_init(PRINT_UART, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
	usart_parity_selection_config(PRINT_UART, USART_PARITY_NONE);
  usart_transmitter_enable(PRINT_UART, TRUE);
	usart_receiver_enable(PRINT_UART, TRUE);
	
	usart_interrupt_enable(PRINT_UART, USART_RDBF_INT, TRUE);


  usart_enable(PRINT_UART, TRUE);

}

void setCallback(CallbackFunction cb)
{
    temp_callback = cb;
}




void USART1_IRQHandler(void)
{

	uint16_t ch;
    if(usart_flag_get(PRINT_UART, USART_RDBF_FLAG) != RESET)
    {
      /* read one byte from the receive data register */
      ch = usart_data_receive(PRINT_UART);			
			temp_callback(ch);
    }
  
}

void uart_send(uint16_t *ch,uint8_t length)
{

	for(uint8_t i = 0;i < length;i++)
	{
		while(usart_flag_get(PRINT_UART, USART_TDBE_FLAG) == RESET){};
		usart_data_transmit(PRINT_UART, *ch);
		ch++;
	}

}


static void dma_configuration(void)
{
  dma_init_type dma_init_struct;

  /* enable dma clock */
  crm_periph_clock_enable(PRINT_UART_DMA_CRM_CLK, TRUE);

  dmamux_enable(DMAx_Port, TRUE);

  /* dma1 channel for usart tx configuration */
  dma_reset(UART_DMA_TX_CH);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = 0;
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
  dma_init_struct.memory_base_addr = (uint32_t)0;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)&PRINT_UART->dt;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(UART_DMA_TX_CH, &dma_init_struct);

  /* config flexible dma for usart tx */
  dmamux_init(DMAMUX_TX_CH, DMAMUX_CH_TO_TXBASE);

  /* dma1 channel for usart rx configuration */
  dma_reset(UART_DMA_RX_CH);
  dma_default_para_init(&dma_init_struct);
  dma_init_struct.buffer_size = 0;
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
  dma_init_struct.memory_base_addr = (uint32_t)0;
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
  dma_init_struct.memory_inc_enable = TRUE;
  dma_init_struct.peripheral_base_addr = (uint32_t)&PRINT_UART->dt;
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
  dma_init_struct.peripheral_inc_enable = FALSE;
  dma_init_struct.priority = DMA_PRIORITY_MEDIUM;
  dma_init_struct.loop_mode_enable = FALSE;
  dma_init(UART_DMA_RX_CH, &dma_init_struct);

  /* config flexible dma for usart rx */
  dmamux_init(DMAMUX_RX_CH, DMAMUX_CH_TO_RXBASE);

  dma_channel_enable(UART_DMA_TX_CH, FALSE); 
  dma_channel_enable(UART_DMA_RX_CH, FALSE); 

}

void uart_dmaPoll_init(uint32_t baudrate)
{
			/* Configure gpio first */
  gpio_init_type gpio_init_struct; 
	
  /* enable the uart and gpio clock */
  crm_periph_clock_enable(PRINT_UART_CRM_CLK, TRUE);
  crm_periph_clock_enable(PRINT_UART_TX_GPIO_CRM_CLK, TRUE);
	crm_periph_clock_enable(PRINT_UART_RX_GPIO_CRM_CLK,TRUE);

  gpio_default_para_init(&gpio_init_struct);

  /* configure the uart tx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = PRINT_UART_TX_PIN;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(PRINT_UART_TX_GPIO, &gpio_init_struct);
	
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = PRINT_UART_RX_PIN;
	gpio_init(PRINT_UART_RX_GPIO, &gpio_init_struct);

  gpio_pin_mux_config(PRINT_UART_TX_GPIO, PRINT_UART_TX_PIN_SOURCE, PRINT_UART_TX_PIN_MUX_NUM);
  gpio_pin_mux_config(PRINT_UART_RX_GPIO, PRINT_UART_RX_PIN_SOURCE, PRINT_UART_RX_PIN_MUX_NUM);

  /* configure uart param */
  usart_init(PRINT_UART, baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
	usart_parity_selection_config(PRINT_UART, USART_PARITY_NONE);
  usart_transmitter_enable(PRINT_UART, TRUE);
	usart_receiver_enable(PRINT_UART, TRUE);
	
	usart_dma_transmitter_enable(PRINT_UART, FALSE);
  usart_dma_receiver_enable(PRINT_UART, FALSE);

  usart_enable(PRINT_UART, TRUE);
	dma_configuration();
}

void uart_dmapoll_send(uint8_t *ch,uint16_t length)
{	
	UART_DMA_TX_CH->maddr = (uint32_t)ch;
	UART_DMA_TX_CH->dtcnt = length;
	usart_dma_transmitter_enable(PRINT_UART,TRUE);          
	dma_channel_enable(UART_DMA_TX_CH, TRUE);            
	while(dma_flag_get(DMA1_FDT1_FLAG)==RESET );        
	dma_flag_clear(DMA1_FDT1_FLAG);                     
	dma_channel_enable(UART_DMA_TX_CH, FALSE);           
	usart_dma_transmitter_enable(PRINT_UART,FALSE);         
}


void uart_dmapoll_read(uint8_t *ch,uint16_t length)
{	
	      
	dma_flag_clear(DMA1_FDT2_FLAG);                 
	dma_channel_enable(UART_DMA_RX_CH, FALSE);       
	usart_dma_receiver_enable(PRINT_UART,FALSE);        
	UART_DMA_RX_CH->maddr = (uint32_t)ch;
	UART_DMA_RX_CH->dtcnt = length;        
	usart_dma_receiver_enable(PRINT_UART,TRUE);         
	dma_channel_enable(UART_DMA_RX_CH, TRUE); 
	while(dma_flag_get(DMA1_FDT2_FLAG)==RESET ); 	
}

void uart_dma_poll_ping(void)
{
	char rx_buff[11];
	uart_dmapoll_read((uint8_t *)&rx_buff,11);
	uart_dmapoll_send((uint8_t *)&rx_buff,11);
}
