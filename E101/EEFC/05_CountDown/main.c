/*
 * project name : 05_CountDown
 * file name    : main.c
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.11.22
 *
 * description  : Implemented a time counter using SSDs.
 *  Attached 4x SSDs and using a state machine, implemented a
 *  time counter with different intervals. Assigned each speed
 *  a mode and attached a button to cycle through the modes.
 *  ( Each button press will cycle through these modes.)
 */

#include "stm32g0xx.h"
#include "bsp.h"

uint8_t refresh =1;
uint8_t state =0;
int8_t Digit1 =10;
int8_t Digit2 =10;
int8_t Digit3 =10;
int8_t Digit4 =10;

void SSDCountDown(void);
void setNumber(int number);
void countdown(void);
void displayDigit(int digit);

int main(void) {
	BSP_ClockEnable();
	BSP_SSDEnable();
	BSP_ButtonEnable();
	BSP_EXITEnable();
	BSP_SysTick();
	BSP_TIM1();
	BSP_TIM3();

	while(1) {
		 // loop
	}
	return 0;
}

void SysTick_Handler(void){
	displayDigit(refresh);
	refresh++;
	if(refresh > 4)
		refresh = 1;
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(){
	SSDCountDown();
	TIM1->SR &= ~(1U<<0);
}

void TIM3_IRQHandler(){
	TIM3->CR1 &= ~(1U<<0);
	GPIOA->MODER &= ~(3U<<2*0);
	TIM3->SR &= ~(1U<<0);
}

void countdown(){
	switch(state){
	case 0:
		TIM1->CR1 &= ~(1U<<0);
		break;
	case 1:
		TIM1->CR1 |= (1U<<0);
		TIM1->PSC=999; //1 sec delay
		break;
	case 2:
		TIM1->CR1 |= (1U<<0);
		TIM1->PSC=499; //.5 sec delay
		break;
	case 3:
		TIM1->CR1 |= (1U<<0);
		TIM1->PSC=99; //.1 sec delay
		break;
	case 4:
		TIM1->CR1 |= (1U<<0);
		TIM1->PSC=9; //.01 sec delay
		break;
	case 5:
		TIM1->CR1 |= (1U<<0);
		TIM1->PSC=0; //.001 sec delay
		break;
	}
}

void SSDCountDown(){
	//reset state
	if(Digit1 == 0 && Digit2 == 0 && Digit3 == 0 && Digit4 == 0){
		Digit1 = 2;
		Digit2 = 3;
		Digit3 = 5;
		Digit4 = 9;
	}
	//Hard Count Down
	if(Digit4 == 0){
		Digit4 = 9;
		Digit3--;
		if(Digit3 < 0){
			Digit3 =  5;
			Digit2--;
			if(Digit2 < 0){
				Digit2 = 9;
				Digit1--;
			}
		}
	}
	//Basic Count Down
	else
		Digit4--;
}

void displayDigit(int digit){
	switch(digit){
		case 1:
			GPIOA->BRR |= (1U <<  9); //D1
			GPIOA->BRR |= (1U << 15); //D2
			GPIOB->BRR |= (1U <<  7); //D3
			GPIOA->BRR |= (1U <<  7); //D4
			GPIOA->ODR |= (1U <<  9); //D1
			BSP_setNumber(Digit1);
			break;
		case 2:
			GPIOA->BRR |= (1U <<  9); //D1
			GPIOA->BRR |= (1U << 15); //D2
			GPIOB->BRR |= (1U <<  7); //D3
			GPIOA->BRR |= (1U <<  7); //D4
			GPIOA->ODR |= (1U << 15); //D2
			BSP_setNumber(Digit2);
			break;
		case 3:
			GPIOA->BRR |= (1U <<  9); //D1
			GPIOA->BRR |= (1U << 15); //D2
			GPIOB->BRR |= (1U <<  7); //D3
			GPIOA->BRR |= (1U <<  7); //D4
			GPIOB->ODR |= (1U <<  7); //D3
			BSP_setNumber(Digit3);
			break;
		case 4:
			GPIOA->BRR |= (1U <<  9); //D1
			GPIOA->BRR |= (1U << 15); //D2
			GPIOB->BRR |= (1U <<  7); //D3
			GPIOA->BRR |= (1U <<  7); //D4
			GPIOA->ODR |= (1U <<  7); //D4
			BSP_setNumber(Digit4);
			break;
	}
}

void EXTI0_1_IRQHandler(void){
	if(Digit1 == 10 && Digit2 == 10 && Digit3 == 10 && Digit4 == 10){
		Digit1 = 2;
		Digit2 = 4;
		Digit3 = 0;
		Digit4 = 0;
	}
	GPIOA->MODER |= (3U << 2*0);
	TIM3->CR1 |= (1U << 0);
	state++;
	if(state > 5)
		state = 0;
	countdown();
	EXTI->RPR1 |= (1U << 0); //clear flag
}
