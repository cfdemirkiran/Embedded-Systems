/*
 * project name : 05_CountDown
 * file name    : bsp.c
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.11.22
 *
 * description  : Enabling some needed GPIOs and Registers for
 *  main file.
 */

#include "stm32g0xx.h"
#include "bsp.h"

void BSP_ClockEnable(){
    /* Enable GPIOA-B clock */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);
}

void BSP_SSDEnable(){
	/* Setup A5-7, A9-12, A15, as output */
	GPIOA->MODER &= ~(3U << 2*5);
	GPIOA->MODER |= (1U << 2*5);
	GPIOA->MODER &= ~(3U << 2*6);
	GPIOA->MODER |= (1U << 2*6);
	GPIOA->MODER &= ~(3U << 2*7);
	GPIOA->MODER |= (1U << 2*7);
	GPIOA->MODER &= ~(3U << 2*9);
	GPIOA->MODER |= (1U << 2*9);
	GPIOA->MODER &= ~(3U << 2*10);
	GPIOA->MODER |= (1U << 2*10);
	GPIOA->MODER &= ~(3U << 2*11);
	GPIOA->MODER |= (1U << 2*11);
	GPIOA->MODER &= ~(3U << 2*12);
	GPIOA->MODER |= (1U << 2*12);
	GPIOA->MODER &= ~(3U << 2*15);
	GPIOA->MODER |= (1U << 2*15);

	/* Setup B1, B6-7 as output */
	GPIOB->MODER &= ~(3U << 2*1);
	GPIOB->MODER |= (1U << 2*1);
	GPIOB->MODER &= ~(3U << 2*6);
	GPIOB->MODER |= (1U << 2*6);
	GPIOB->MODER &= ~(3U << 2*7);
	GPIOB->MODER |= (1U << 2*7);
}

void BSP_ButtonEnable(){
	/* Setup PA0 as input */
	GPIOA->MODER &= ~(3U << 2*0);
	GPIOA->PUPDR &= ~(3U << 2*0);
	GPIOA->PUPDR |= (1U << 2*0);
}

void BSP_EXITEnable(){
	/* EXTI at PA0 port */
	EXTI->EXTICR[0] |= (0U << 8*0);

	/* Rising edge setup */
	EXTI->RTSR1 |= (1U << 0);

	/* Mask setup */
	EXTI->IMR1 |= (1U << 0);

	/* NVIC setup */
	NVIC_SetPriority(EXTI0_1_IRQn, 1);
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void BSP_SysTick(){
	/* System Clock */
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 500);
}

void BSP_TIM1(){
	/* Timer Clock */
	RCC->APBENR2 |= (1U << 11); //TIM1

	/* NVIC setup */
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 2);
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

	/* Timer1 */
	TIM1->CR1 = 0;
	TIM1->CR1 |= (1U<<7);
	TIM1->CNT = 0;
	TIM1->ARR=16000;
	TIM1->DIER |= (1U<<0);

}

void BSP_TIM3(){
	/* Timer Clock*/
	RCC->APBENR1 |= (1U << 1); //TIM2

	/* NVIC setup */
	NVIC_SetPriority(TIM3_IRQn, 0);
	NVIC_EnableIRQ(TIM3_IRQn);

	/* Timer3 */
	TIM3->CR1 = 0;
	TIM3->CR1 |= (1U << 7);
	TIM3->CNT = 0;
	TIM3->ARR=16000;
	TIM3->PSC=999;
	TIM3->DIER |= (1U<<0);
}

void BSP_setNumber(int number){
	GPIOA->BRR |= (1U << 10); //A
	GPIOB->BRR |= (1U <<  6); //B
	GPIOA->BRR |= (1U << 11); //C
	GPIOA->BRR |= (1U << 12); //D
	GPIOA->BRR |= (1U <<  5); //E
	GPIOB->BRR |= (1U <<  1); //F
	GPIOA->BRR |= (1U <<  6); //G
	switch(number){
	case 0:
		GPIOA->ODR |= (1U << 06); //G
		break;
	case 1:
		GPIOA->ODR |= (1U << 10); //A
		GPIOA->ODR |= (1U << 12); //D
		GPIOA->ODR |= (1U << 05); //E
		GPIOB->ODR |= (1U << 01); //F
		GPIOA->ODR |= (1U << 06); //G
		break;
	case 2:
		GPIOB->ODR |= (1U << 01); //F
		GPIOA->ODR |= (1U << 11); //C
		break;
	case 3:
		GPIOA->ODR |= (1U << 05); //E
		GPIOB->ODR |= (1U << 01); //F
		break;
	case 4:
		GPIOA->ODR |= (1U << 10); //A
		GPIOA->ODR |= (1U << 12); //D
		GPIOA->ODR |= (1U << 05); //E
		break;
	case 5:
		GPIOB->ODR |= (1U << 06); //B
		GPIOA->ODR |= (1U << 05); //E
		break;
	case 6:
		GPIOB->ODR |= (1U << 06); //B
		break;
	case 7:
		GPIOA->ODR |= (1U << 12); //D
		GPIOA->ODR |= (1U << 05); //E
		GPIOB->ODR |= (1U << 01); //F
		GPIOA->ODR |= (1U << 06); //G
		break;
	case 8:
		break;
	case 9:
		GPIOA->ODR |= (1U << 05); //E
		break;
	case 10:
		GPIOA->ODR |= (1U << 10); //A
		GPIOB->ODR |= (1U << 06); //B
		GPIOA->ODR |= (1U << 11); //C
		GPIOA->ODR |= (1U << 12); //D
		GPIOA->ODR |= (1U << 05); //E
		GPIOB->ODR |= (1U << 01); //F
	}
}
