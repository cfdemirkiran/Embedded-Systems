/*
 * project name : _01TEST_EXTIbuttonSM
 * file name    : main.c
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : -
 *
 * description  : -
 */

#include "stm32g0xx.h"

/* 1 Sec is 1600000 */
void delay(volatile uint32_t);
void KeypadAllRows_SET(void);
void KeypadAllRows_RESET(void);
void SSD_RESET(void);
void SSD_SET(volatile uint8_t);
void SSD_Close(void);

volatile uint8_t Counter = 0;
volatile uint8_t KeyPress;
volatile uint8_t SSD_Digit;
volatile uint8_t SSD_Digit1 = 0;
volatile uint8_t SSD_Digit2 = 0;
volatile uint8_t SSD_Digit3 = 0;
volatile uint8_t SSD_Digit4 = 0;

/* Interrupt Handler */
	void EXTI4_15_IRQHandler(void){
		/* keypad press from C1 */
		KeypadAllRows_RESET();
		GPIOA->ODR |= (1U << 8);       //Row1
		if((GPIOB->IDR & (1U << 4)) == (1U << 4)){      //1
			KeyPress = 1;

		}
		else if((GPIOB->IDR & (1U << 5)) == (1U << 5)){ //2
			KeyPress = 2;
		}
		else if((GPIOB->IDR & (1U << 9)) == (1U << 9)){ //3
			KeyPress = 3;
		}

		KeypadAllRows_RESET();
		GPIOB->ODR |= (1U << 8);       //Row2
		if((GPIOB->IDR & (1U << 4)) == (1U << 4)){      //4
			KeyPress = 4;
		}
		else if((GPIOB->IDR & (1U << 5)) == (1U << 5)){ //5
			KeyPress = 5;
		}
		else if((GPIOB->IDR & (1U << 9)) == (1U << 9)){ //6
			KeyPress = 6;
		}

		KeypadAllRows_RESET();
		GPIOB->ODR |= (1U << 2);       //Row3
		if((GPIOB->IDR & (1U << 4)) == (1U << 4)){      //7
			KeyPress = 7;
		}
		else if((GPIOB->IDR & (1U << 5)) == (1U << 5)){ //8
			KeyPress = 8;
		}
		else if((GPIOB->IDR & (1U << 9)) == (1U << 9)){ //9
			KeyPress = 9;
		}

		KeypadAllRows_RESET();
		GPIOB->ODR |= (1U << 0);       	//Row4
		if((GPIOB->IDR & (1U << 5)) == (1U << 5)){      //2
			KeyPress = 0;
		}

		SSD_Digit1 = SSD_Digit2;
    	SSD_Digit2 = SSD_Digit3;
    	SSD_Digit3 = SSD_Digit4;
    	SSD_Digit4 = KeyPress;

		EXTI->RPR1 |= (1U << 4);
		EXTI->RPR1 |= (1U << 5);
		EXTI->RPR1 |= (1U << 9);
		KeypadAllRows_SET();
	}


int main(void) {

    /* Enable GPIOA, GPIOB clock */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);

	EXTI->RPR1 |= (1U << 4);
	EXTI->RPR1 |= (1U << 5);
	EXTI->RPR1 |= (1U << 9);

    /* Setup PA8, PB8, PB2, PB0 as output for keypad */
    GPIOA->MODER &= ~(3U << 2*8); //PA8
    GPIOA->MODER |= (1U << 2*8);

    GPIOB->MODER &= ~(3U << 2*8); //PB8
    GPIOB->MODER |= (1U << 2*8);

    GPIOB->MODER &= ~(3U << 2*2); //PB2
    GPIOB->MODER |= (1U << 2*2);

    GPIOB->MODER &= ~(3U << 2*0); //PB0
    GPIOB->MODER |= (1U << 2*0);

    /* Setup PB4, PB5, PB9 as input for keypad */
    GPIOB->MODER &= ~(3U << 2*4);
    GPIOB->PUPDR |= (2U << 2*4);

    GPIOB->MODER &= ~(3U << 2*5);
    GPIOB->PUPDR |= (2U << 2*5);

    GPIOB->MODER &= ~(3U << 2*9);
    GPIOB->PUPDR |= (2U << 2*9);

    /* EXTI at PB4, PB5, PB9 port */
    EXTI->EXTICR[1] |= (1U << 8*0);
    EXTI->EXTICR[1] |= (1U << 8*1);
    EXTI->EXTICR[2] |= (1U << 8*1);

    /* Rising edge setup */
    EXTI->RTSR1 |= (1U << 5);
    EXTI->RTSR1 |= (1U << 4);
    EXTI->RTSR1 |= (1U << 9);

    /* Mask setup */
    EXTI->IMR1  |= (1U << 4);
    EXTI->IMR1  |= (1U << 5);
    EXTI->IMR1  |= (1U << 9);

    /* Setup NVIC */
    NVIC_SetPriority(EXTI4_15_IRQn, 0);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

    /* Setup PA5-7, PA9-12, PA15, PB1, PB6-7 as output for SSD */
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

    GPIOB->MODER &= ~(3U << 2*1);
    GPIOB->MODER |= (1U << 2*1);

    GPIOB->MODER &= ~(3U << 2*6);
    GPIOB->MODER |= (1U << 2*6);

    GPIOB->MODER &= ~(3U << 2*7);
    GPIOB->MODER |= (1U << 2*7);


    KeypadAllRows_SET();

    while(1) {
    	/* LOOP */
    	SSD_Close();
    	GPIOA->ODR |= (1U <<  7); //D4
    	SSD_SET(SSD_Digit4);
    	delay(100);

    	SSD_Close();
    	GPIOB->ODR |= (1U <<  7); //D3
    	SSD_SET(SSD_Digit3);
    	delay(100);

    	SSD_Close();
    	GPIOA->ODR |= (1U << 15); //D2
    	SSD_SET(SSD_Digit2);
    	delay(100);

    	SSD_Close();
    	GPIOA->ODR |= (1U <<  9); //D1
    	SSD_SET(SSD_Digit1);
    	delay(100);

    }

    return 0;
}
/* All Functions */

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}

void KeypadAllRows_SET(){
	/* Engage All Keypad Rows */
	GPIOB->ODR |= (1U << 0);
	GPIOB->ODR |= (1U << 2);
	GPIOB->ODR |= (1U << 8);
	GPIOA->ODR |= (1U << 8);
}

void KeypadAllRows_RESET(){
	GPIOB->BRR |= (1U << 0);
	GPIOB->BRR |= (1U << 2);
	GPIOB->BRR |= (1U << 8);
	GPIOA->BRR |= (1U << 8);
}

void SSD_SET (uint8_t x){
	SSD_RESET();
	switch(x){
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
	}

}

void SSD_RESET(){
	GPIOA->BRR |= (1U << 10); //A
	GPIOB->BRR |= (1U <<  6); //B
	GPIOA->BRR |= (1U << 11); //C
	GPIOA->BRR |= (1U << 12); //D
	GPIOA->BRR |= (1U <<  5); //E
	GPIOB->BRR |= (1U <<  1); //F
	GPIOA->BRR |= (1U <<  6); //G
}

void SSD_SETUP(){
	GPIOA->ODR |= (1U <<  9); //D1
	GPIOA->ODR |= (1U << 15); //D2
	GPIOB->ODR |= (1U <<  7); //D3
	GPIOA->ODR |= (1U <<  7); //D4
}

void SSD_Close(){
	GPIOA->ODR = (0U <<  9); //D1
	GPIOA->ODR = (0U << 15); //D2
	GPIOB->ODR = (0U <<  7); //D3
	GPIOA->ODR = (0U <<  7); //D4
}