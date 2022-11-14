
/*==================================================================================================================================*/
#include "utility.h"
#include "stm32f0xx.h"
#include "main.h"
#include "communication.h"
/*==================================================================================================================================*/
/*==================================================================================================================================*/
/*==================================================================================================================================*/
/**
  * @brief  General purpose timer, 10 ms interrupt interval
  * @param  None:
  * @retval None:
  */
void timer14_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable timer clock  - use TIMER2 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	/* 10 ms interrupt */
	TIM_TimeBaseStructure.TIM_Prescaler = SYS_CLK / 1100;
	TIM_TimeBaseStructure.TIM_Period = 10;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Timer Interrupt	*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* NVIC Init	*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;			
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable counter */
	TIM_Cmd(TIM14, ENABLE);
}
/*==================================================================================================================================*/
/**
  * @brief  Timer interrupt
  * @param: None
  * @retval: None
  */
void TIM14_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		/* Clear the TIM2 interrupt */
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);		
		communication_timer_function();

	}
}
/*==================================================================================================================================*/
/**
  * @brief  delay_ms_timer_init, 1 ms interrupt period
  * @param  None:
  * @retval None:
  */
void timer3_init(void) 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/* Enable timer clock  - use TIMER3 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* 1 ms resolution */
	TIM_TimeBaseStructure.TIM_Prescaler = SYS_CLK / 1000;
	TIM_TimeBaseStructure.TIM_Period = 10000;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* Enable counter */
	TIM_Cmd(TIM3, ENABLE);
}
/*==================================================================================================================================*/
/**
  * @brief  delay_us_timer_init, 1 us interrupt period
  * @param  None:
  * @retval None:
  */
void timer6_init(void) 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/* Enable timer clock  - use TIMER6 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	/* 1 us resolution */
	TIM_TimeBaseStructure.TIM_Prescaler = 50;
	TIM_TimeBaseStructure.TIM_Period = 10000;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	/* Enable counter */
	TIM_Cmd(TIM6, ENABLE);
}
/*==================================================================================================================================*/

/*==================================================================================================================================*/
/**
  * @brief  delay function in miliseconds
  * @param  msecs: Parameter as milisecond
  * @retval None:
  */
void delay_ms(uint16_t msecs)
{
	TIM_SetCounter(TIM3, 0);
	
	while (TIM_GetCounter(TIM3) < msecs)
		;
}
/*==================================================================================================================================*/
/**
  * @brief  delay function in microseconds
  * @param  usecs: Parameter as microsecond
  * @retval None:
  */
void delay_us(uint16_t usecs)
{
	TIM_SetCounter(TIM6, 0);
	
	while (TIM_GetCounter(TIM6) < usecs)
		;
}

/*==================================================================================================================================*/
/**
  * @brief  timer init
  * @retval None:
  */
void timer_init()
{
	timer14_init();
	timer3_init();
	timer6_init();
}

/*==================================================================================================================================*/

uint8_t checksum(const uint8_t *buf, uint16_t size)
{
	uint8_t checksum; 
	
	checksum = *buf, size--;
	while (size--)
		checksum ^= *++buf;
	
   return checksum;
}
/*==================================================================================================================================*/
char hex_to_ascii(uint8_t val, _Bool high_low)
{
	if (high_low) {
		val /= 16;
		if (val > 9)
			return val + 55;
		return val + 48;
	} else {
		val %= 16;
		if (val > 9)
			return val + 55;
		return val + 48;
	}
}
/*==================================================================================================================================*/
static uint8_t
nibbleFromChar(char c)
{
	if(c >= '0' && c <= '9') return c - '0';
	if(c >= 'a' && c <= 'f') return c - 'a' + 10;
	if(c >= 'A' && c <= 'F') return c - 'A' + 10;
	return 255;
}

/* Convert a string of characters representing a hex buffer into a series of bytes of that real value */
uint8_t hexStringToUint8(uint8_t low , uint8_t high)
{
	uint8_t retval;	
	
	retval = (nibbleFromChar(high) << 4) | nibbleFromChar(low);
	
  return retval;
}
/*==================================================================================================================================*/
void checksum_test(void)
{
	const uint8_t message[] = "$GPRMC,064951.000,A,2307.1256,N,12016.4438,E,0.03,165.48,260406,3.05,W,A*2C\x13\x10";
	uint8_t checksum_val;
	
	checksum_val = checksum(message + 1, sizeof(message) - 7);
	checksum_val++;
}
/*==================================================================================================================================*/
