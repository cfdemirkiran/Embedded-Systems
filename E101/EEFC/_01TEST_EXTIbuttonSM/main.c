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
void Keypad_detect();

volatile uint32_t keypadPress = 10;
volatile uint32_t SSDdigit  = 0;
volatile uint32_t SSDdigit1;
volatile uint32_t SSDdigit2;
volatile uint32_t SSDdigit3;
volatile uint32_t SSDdigit4;


int main(void) {

    /* Enable GPIOA, GPIOB clock */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);

    /* Setup PA5-12, PA15, PB1, PB0, PB2, PB6-7, PB8 as output */
    GPIOA->MODER &= ~(3U << 2*5);
    GPIOA->MODER |= (1U << 2*5);

    GPIOA->MODER &= ~(3U << 2*6);
    GPIOA->MODER |= (1U << 2*6);

    GPIOA->MODER &= ~(3U << 2*7);
    GPIOA->MODER |= (1U << 2*7);

    GPIOA->MODER &= ~(3U << 2*8);
    GPIOA->MODER |= (1U << 2*8);

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

    GPIOB->MODER &= ~(3U << 2*0);
    GPIOB->MODER |= (1U << 2*0);

    GPIOB->MODER &= ~(3U << 2*2);
    GPIOB->MODER |= (1U << 2*2);

    GPIOB->MODER &= ~(3U << 2*6);
    GPIOB->MODER |= (1U << 2*6);

    GPIOB->MODER &= ~(3U << 2*7);
    GPIOB->MODER |= (1U << 2*7);

    GPIOB->MODER &= ~(3U << 2*8);
    GPIOB->MODER |= (1U << 2*8);

    /* Setup PB4, PB5, PB9 as input */
    GPIOB->MODER &= ~(3U << 2*4);
    GPIOB->MODER |= (1U << 2*4);

    GPIOB->MODER &= ~(3U << 2*5);
    GPIOB->MODER |= (1U << 2*5);

    GPIOB->MODER &= ~(3U << 2*9);
    GPIOB->MODER |= (1U << 2*9);

    /* EXTI at PB4, PB5, PB9 port */
    EXTI->EXTICR[1] |= (1U << 8*0);
    EXTI->EXTICR[1] |= (1U << 8*1);
    EXTI->EXTICR[2] |= (1U << 8*1);

    /* Mask and Rising on Px4, 5, 9 */
    EXTI->IMR1  |= (1U << 4);
    EXTI->RTSR1 |= (1U << 4);

    EXTI->IMR1  |= (1U << 5);
    EXTI->RTSR1 |= (1U << 5);

    EXTI->IMR1  |= (1U << 9);
    EXTI->RTSR1 |= (1U << 9);

    /* Setup NVIC */
    NVIC_SetPriority(EXTI4_15_IRQn, 0);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

	/* Engage All Keypad Rows */
	GPIOB->ODR |= (1U << 0);
	GPIOB->ODR |= (1U << 2);
	GPIOB->ODR |= (1U << 8);
	GPIOA->ODR |= (1U << 8);

    /* SSD Functions */
    void SSD_0 (){

    	GPIOA->ODR |= (1U << 06); //G
    }

    void SSD_1(){

    	GPIOA->ODR |= (1U << 10); //A
    	GPIOA->ODR |= (1U << 12); //D
    	GPIOA->ODR |= (1U << 05); //E
    	GPIOB->ODR |= (1U << 01); //F
    	GPIOA->ODR |= (1U << 06); //G
    }

    void SSD_2(){

    	GPIOB->ODR |= (1U << 01); //F
    	GPIOA->ODR |= (1U << 11); //C
    }

    void SSD_3(){

    	GPIOA->ODR |= (1U << 05); //E
    	GPIOB->ODR |= (1U << 01); //F
    }

    void SSD_4(){

    	GPIOA->ODR |= (1U << 10); //A
    	GPIOA->ODR |= (1U << 12); //D
    	GPIOA->ODR |= (1U << 05); //E
    }

    void SSD_5(){

    	GPIOB->ODR |= (1U << 06); //B
    	GPIOA->ODR |= (1U << 05); //E
    }

    void SSD_6(){

    	GPIOB->ODR |= (1U << 06); //B
    }

    void SSD_7(){

    	GPIOA->ODR |= (1U << 12); //D
    	GPIOA->ODR |= (1U << 05); //E
    	GPIOB->ODR |= (1U << 01); //F
    	GPIOA->ODR |= (1U << 06); //G
    }

    void SSD_8(){

    }

    void SSD_9(){

    	GPIOA->ODR |= (1U << 05); //E
    }

    void SSD_set(){

    	GPIOA->ODR |= (1U << 10); //A
    	GPIOB->ODR |= (1U << 06); //B
    	GPIOA->ODR |= (1U << 11); //C
    	GPIOA->ODR |= (1U << 12); //D
    	GPIOA->ODR |= (1U << 05); //E
    	GPIOB->ODR |= (1U << 01); //F
    	GPIOA->ODR |= (1U << 06); //G
    }

	void Keypad_detect(){
	/* Column 1 Detected */
	if ((GPIOB->IDR & (1U << 4)) == (1U << 4)){

		GPIOB->ODR = (0U << 2);
		GPIOB->ODR = (0U << 8);
		GPIOB->ODR = (0U << 0);

		if ((GPIOB->IDR & (1U << 4)) == (1U << 4)){

			SSDdigit++;
			keypadPress = 1;
		}

		else{

			GPIOA->ODR  = (0U << 8);
	    	GPIOB->ODR |= (1U << 8);

	    	if ((GPIOB->IDR & (1U << 4)) == (1U << 4)){

				SSDdigit++;
	    		keypadPress = 4;
	    	}

	    	else{

				GPIOB->ODR  = (0U << 8);
		    	GPIOB->ODR |= (1U << 2);
		    	if((GPIOB->IDR & (1U << 4)) == (1U << 4)){

					SSDdigit++;
		    		keypadPress = 7;
		    	}

		    	else{

		    		/* Pressed * */
		    	}
	    	}


		}
	}

	/* Column 2 Detected */
	else if ((GPIOB->IDR & (1U << 5)) == (1U << 5)){

		GPIOB->ODR = (0U << 2);
		GPIOB->ODR = (0U << 8);
		GPIOB->ODR = (0U << 0);

		if ((GPIOB->IDR & (1U << 5)) == (1U << 5)){

			SSDdigit++;
			keypadPress = 2;
		}

		else{

			GPIOA->ODR  = (0U << 8);
	    	GPIOB->ODR |= (1U << 8);

	    	if ((GPIOB->IDR & (1U << 5)) == (1U << 5)){

				SSDdigit++;
	    		keypadPress = 5;
	    	}

	    	else{

				GPIOB->ODR  = (0U << 8);
		    	GPIOB->ODR |= (1U << 2);
		    	if((GPIOB->IDR & (1U << 5)) == (1U << 5)){

					SSDdigit++;
		    		keypadPress = 8;
		    	}

		    	else{

					SSDdigit++;
		    		keypadPress = 0;
		    	}
	    	}
		}

	}

	/* Column 3 Detected */
	else{

		GPIOB->ODR = (0U << 2);
		GPIOB->ODR = (0U << 8);
		GPIOB->ODR = (0U << 0);

		if ((GPIOB->IDR & (1U << 9)) == (1U << 9)){

			SSDdigit++;
			keypadPress = 3;
		}

		else{

			GPIOA->ODR  = (0U << 8);
	    	GPIOB->ODR |= (1U << 8);

	    	if ((GPIOB->IDR & (1U << 9)) == (1U << 9)){

				SSDdigit++;
	    		keypadPress = 6;
	    	}

	    	else{

				GPIOB->ODR  = (0U << 8);
		    	GPIOB->ODR |= (1U << 2);
		    	if((GPIOB->IDR & (1U << 9)) == (1U << 9)){

					SSDdigit++;
		    		keypadPress = 9;
		    	}

		    	else{

		    		/* Pressed # */
		    	}
	    	}


		}
	}

	/* Engage Again */
	GPIOB->ODR |= (1U << 0);
	GPIOB->ODR |= (1U << 2);
	GPIOB->ODR |= (1U << 8);
	GPIOA->ODR |= (1U << 8);

	}

    /* Keypad detector */
    void KeyPress ( uint32_t x ){

    	if ( x == 0){

    		SSD_0();
    	}
    	else if ( x == 1){

    		SSD_1();
    	}
    	else if ( x == 2){

    		SSD_2();
    	}
    	else if ( x == 3){

    		SSD_3();
    	}
    	else if ( x == 4){

    		SSD_4();
    	}
    	else if ( x == 5){

    		SSD_5();
    	}
    	else if ( x == 6){

    		SSD_6();
    	}
    	else if ( x == 7){

    		SSD_7();
    	}
    	else if ( x == 8){

    		SSD_8();
    	}
    	else if ( x == 9){

    		SSD_9();
    	}

    }

    /* SSD Fire Up Function */
	void SSD_on_fire(){
	/* SSD Digit Loop Function */
	if(SSDdigit == 1){

		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOA->ODR |= (1U << 9); //D1
		KeyPress( keypadPress );
		keypadPress = SSDdigit1;
		delay(100);
	}

	if(SSDdigit == 2){

		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOA->ODR |= (1U << 9); //D1
		KeyPress(keypadPress);
		delay(100);

		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOA->ODR |= (1U << 15); //D2
		KeyPress(SSDdigit1);
		SSDdigit2 = SSDdigit1;
		SSDdigit1 = keypadPress;
		delay(100);
	}

	if(SSDdigit == 3){

		/* Digit 1 */
		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOA->ODR |= (1U << 9); //D1
		KeyPress(keypadPress);
		delay(100);

		/* Digit 2 */
		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOA->ODR |= (1U << 15); //D2
		KeyPress(SSDdigit1);
		delay(100);

		/* Digit 3 */
		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOB->ODR |= (1U << 7); //D3
		KeyPress(SSDdigit2);
		SSDdigit3 = SSDdigit2;
		SSDdigit2 = SSDdigit1;
		SSDdigit1 = keypadPress;
		delay(100);
	}

	if(SSDdigit >= 4){

		/* Digit 1 */
		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOA->ODR |= (1U << 9); //D1
		KeyPress(keypadPress);
		delay(100);

		/* Digit 2 */
		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOA->ODR |= (1U << 15); //D2
		KeyPress(SSDdigit1);
		delay(100);

		/* Digit 3 */
		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOB->ODR |= (1U << 07); //D3
		KeyPress(SSDdigit2);
		delay(100);


		/* Digit 4 */
		GPIOA->ODR  = (0U << 9); //D1
		GPIOA->ODR  = (0U << 15); //D2
		GPIOB->ODR  = (0U << 7); //D3
		GPIOA->ODR  = (0U << 7); //D4

		GPIOA->ODR |= (1U << 7); //D4
		KeyPress(SSDdigit3);
		SSDdigit3 = SSDdigit2;
		SSDdigit2 = SSDdigit1;
		SSDdigit1 = keypadPress;
		delay(100);
	}
	}

	/* Interrupt Handler */
	void EXTI4_15_IRQHandler(void){

		Keypad_detect();

	}
    while(1) {


    	/* LOOP */
    	SSD_on_fire();
    }
    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}
