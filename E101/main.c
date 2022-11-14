/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    11-April-2014
  * @brief   Main program body
  ******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dgps.h"
#include "peripheral_init.h"
#include "utility.h"
#include "communication.h"
#include <stdio.h>

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
uint8_t com_handler_message_ready ;
extern struct configuration Configuration;
	
int main(void)
{
  mcu_setup();
	io_init();
	timer14_init();
	timer3_init();
	timer6_init();
	usart12_init();
	
	//initialize serial communication
	usart1_protocol_setup();
  STM_EVAL_LEDInit(LED2);
	
	uint8_t conf ;
	
	// try to configure n times and ligth led
	for(int i = 0 ; i < 5 ; i++ )
	{
		conf = configureDGPS( (uint8_t *)&Configuration );
		if(!conf)
			break;
	}
	if(conf)
		STM_EVAL_LEDOff(LED2);
	else
		STM_EVAL_LEDOn(LED2);
	
	while (1)
  {
		check_nmea0183_message();
		slave_check_and_evaluate_receive_message();
	}
}

