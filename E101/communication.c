/*==================================================================================================================================*/
#include "communication.h"
#include "stm32f0xx_usart.h"
#include "svs_communication.h"
#include "peripheral_init.h"
#include "dgps.h"
#include <stdio.h>
#include <string.h>
#include "utility.h"
#include "main.h"
/*==================================================================================================================================*/
struct com_handler_t com_handler_slave;
static uint8_t data[BUFFER_LIMIT];
uint8_t operationMode;
struct dgpsData DGPSData = {.heading=0.0 , .Position={0.0,0.0}};

/*==================================================================================================================================*/
/**
  * @brief  usart_config
  * @param  None:
  * @retval None:
  */
void usart12_init(void)
{
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	// USART1 Clock Enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	
	// USART1 Init
	USART_InitStruct.USART_BaudRate = 19200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART1, &USART_InitStruct);
	
	// USART1 Interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// USART2 Init
	USART_InitStruct.USART_BaudRate = 19200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	USART_Init(USART2, &USART_InitStruct);
	
	// USART2 Interrupt
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);


	// NVIC Init	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable USART1,2
	USART_Cmd(USART1,ENABLE);  
	USART_Cmd(USART2,ENABLE);  

}
/*==================================================================================================================================*/
/**
  * @brief  This function handles UART1 Channel interrupt request.
  * @param: None
  * @retval: None
  */
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		uint8_t temp;
		/* Clear the USART1 Receive interrupt */
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		temp = USART_ReceiveData(USART1) & 0xFF;		 
	  nmea0183_message_parser(temp);
	}
	
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET) {
		/* Clear overrun flag */
		USART_ClearFlag(USART1, USART_FLAG_ORE);
	}	
}
/*==================================================================================================================================*/
/**
  * @brief  This function handles USART2 Channel interrupt request.
  * @param: None
  * @retval: None
  */
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		uint8_t temp;
		/* Clear the USART2 Receive interrupt */
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		temp = USART_ReceiveData(USART2) & 0xFF;		 
		svs_communication_parse_message(&com_handler_slave, temp);
	}
	
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET) {
		/* Clear overrun flag */
		USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
}
/*==================================================================================================================================*/
/**
  * @brief  This function puts characters over UART1 channel
  * @param: None
  * @retval: None
  */
void uart1_putnc(const uint8_t *data, size_t size)
{
	while (size--) {
		USART_SendData(USART1, *data++);
		/* Wait for transmission complete (USART_FLAG_TC)	*/
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET || !USART_GetFlagStatus(USART1, USART_FLAG_TC))
			;
	}
}
/*==================================================================================================================================*/
/**
  * @brief  This function puts characters over UART2 channel
  * @param: None
  * @retval: None
  */
void uart2_putnc(const uint8_t *data, size_t size)
{
	while (size--) {
		USART_SendData(USART2, *data++);
		/* Wait for transmission complete (USART_FLAG_TC)	*/
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET || !USART_GetFlagStatus(USART2, USART_FLAG_TC))
			;
	}
}
/*==================================================================================================================================*/
/**
  * @brief  This function configures custom SVS protocol parameters.
  * @param: None
  * @retval: None
  */
void usart1_protocol_setup(void)
{
	com_handler_slave.checksum_func = checksum_crc;
	com_handler_slave.rx_com_header = 2;
	com_handler_slave.tx_com_header = 6;	
	com_handler_slave.device_id = 40;
	com_handler_slave.rs485_adr = 12;
	com_handler_slave.putnc = uart2_putnc;
	com_handler_slave.rx_blinker = NULL;
	com_handler_slave.rx_blink_off = NULL;
	com_handler_slave.tx_blinker = NULL;
	com_handler_slave.tx_blink_off = NULL;
	com_handler_slave.rs485_tx_en = 0;
	com_handler_slave.common_rs485_address = 1;
	com_handler_slave.MESSAGE_DISCARD_TIMEOUT = TIMER_200MS; 
	com_handler_slave.check_slave_rs485_address = 11;
}
/*==================================================================================================================================*/
/**
  * @brief  This function handles datas coming from master device.
  * @param: None
  * @retval: None
  */
void slave_check_and_evaluate_receive_message(void)
{
	uint8_t command = 0;

	command = check_message(&com_handler_slave, data);
	//uart2_putnc(&command_id , 1);
	
	if(command == 1)
	{
		if(operationMode == 2)
			return;
		
		send_message(&com_handler_slave , command , (uint8_t *)&DGPSData , (uint16_t)sizeof(DGPSData) );
		operationMode =1; 
		//send status struct
	}
	if(command == 2)
	{
		operationMode = 2;// Configuration mode
		
		STM_EVAL_LEDOff(LED2);
		uint8_t err= configureDGPS(data);
		STM_EVAL_LEDOn(LED2);
		
		uint8_t conf_response;
		uint16_t resp_size = 1;
		if(err)
		{
			conf_response = 0 ;
			send_message(&com_handler_slave , command , &conf_response , resp_size);
		}
		else
		{
			conf_response = 1 ;
			send_message(&com_handler_slave , command , &conf_response , resp_size );
		}			
		
		operationMode = 1;
		
	}	

	// request nmea0183 formatted native message 
	if ( command == 61 ) {		
		uint8_t gpsData[100] ; 
		uint16_t gpsDataIndex;
		
		if(com_handler_message_ready)
		{
			gpsData2Send(gpsData , &gpsDataIndex);
			command = 21; 
			send_message(&com_handler_slave, command,gpsData, gpsDataIndex);			
			com_handler_message_ready = 0;

		}
	}
	
}
/*==================================================================================================================================*/
/**
  * @brief  Invoke this function in timer interrupt.
  * @param: None
  * @retval: None
  */
void communication_timer_function(void)
{
	svs_communication_system_timer_function(&com_handler_slave);
}

/*==================================================================================================================================*/

