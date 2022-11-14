
/*==================================================================================================================================*/
#include "stm32f0xx.h"
#include "peripheral_init.h"
/*==================================================================================================================================*/
/**
  * @brief  usart1 tx/rx port initialization
  * @param  None:
  * @retval None:
  */
static void usart1_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	// RX TX Port Init
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;		// PA9 : USART1_Tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				// PA10 : USART1_Rx
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Alternate Function Init
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); // USART1 TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); // USART1 RX
	
}
/*==================================================================================================================================*/
/**
  * @brief  usart1 tx/rx port initialization
  * @param  None:
  * @retval None:
  */
static void usart2_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* GPIOA Periph clock enable */
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	// RX TX Port Init
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;		// PA2 : USART2_Tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;							// PA3 : USART2_Rx
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Alternate Function Init
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1); // USART2 TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1); // USART2 RX
	
}

void io_init(void)
{
	usart1_io_init();
	usart2_io_init();
}


/*==================================================================================================================================*/
/**
  * @brief  mcu init, 48 Mhz
  * @param  None:
  * @retval None:
  */
void mcu_setup(void)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_OFF);
    RCC_HSICmd(ENABLE);
	  //RCC_WaitForHSEStartUp();
	
    FLASH_PrefetchBufferCmd(ENABLE);
    FLASH_SetLatency(FLASH_Latency_1);
	
	/*	Configures the AHB clock (HCLK).	*/
	/*	RCC_SYSCLK_Div1:   AHB clock = SYSCLK	*/
    RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
	/*	Configures the PLL clock source and multiplication factor.	*/
	/*	RCC_PLLSource_PREDIV1: PREDIV1 clock selected as PLL clock entry	*/
	/*!< PLL input clock*12 */
	/* Set to 48 MHz	*/
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_CFGR_PLLMULL12);
	
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		;
	
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08)
		;
	
	SystemCoreClockUpdate();
}
