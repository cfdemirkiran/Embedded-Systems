/*
 * project name : 02_SMbuttonLED
 * file name    : main.c
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.10.27
 *
 * description   : A state machine blink the external LED at different intervals.
 *   Assigned each speed to a mode, and attach a button to cycle through the modes.
 *   States are created by if-else structure. Each button press will cycle through
 *   these modes.
 */

#include "stm32g0xx.h"

#define LEDDELAY    1600000

void delay(volatile uint32_t);

int main(void) {

	uint32_t ButtonPress = 0;

    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U << 0);

    /* Setup PA0 as output */
    GPIOA->MODER &= ~(3U << 2*0);
    GPIOA->MODER |= (1U << 2*0);

    /* Setup PA1 as input */
    GPIOA->MODER &= ~(3U << 2*1);
    GPIOA->MODER |= (0U << 2*1);


    while(1) {

    	if((GPIOA->IDR & (1U << 1)) == (1U << 1))
    		ButtonPress++;

    	if(ButtonPress == 0)
    		/* Turn off LED */
            GPIOA->ODR = (0U << 0);

    	else if(ButtonPress == 1){
    	    /* Turn on LED 2sec interval */
    	    GPIOA->ODR |= (1U << 0);
    	    delay(LEDDELAY);
    	    delay(LEDDELAY);
    	    GPIOA->ODR ^= (1U << 0);
    	    delay(LEDDELAY);
    	    delay(LEDDELAY);
    	}

    	else if(ButtonPress == 2){
    	    /* Turn on LED 1sec interval */
    	    GPIOA->ODR |= (1U << 0);
    	    delay(LEDDELAY);
    	    GPIOA->ODR ^= (1U << 0);
    	    delay(LEDDELAY);
    	}

    	else if(ButtonPress == 3){
    	    /* Turn on LED 0.5sec interval */
    	    GPIOA->ODR |= (1U << 0);
    	    delay(LEDDELAY/2);
    	    GPIOA->ODR ^= (1U << 0);
    	    delay(LEDDELAY/2);
    	}

    	else if(ButtonPress == 4){
    	    /* Turn on LED 0.1sec interval */
    	    GPIOA->ODR |= (1U << 0);
    	    delay(LEDDELAY/10);
    	    GPIOA->ODR ^= (1U << 0);
    	    delay(LEDDELAY/10);
    	}

    	else if(ButtonPress == 5){
    	    /* Turn on LED */
    	    GPIOA->ODR |= (1U << 0);
    	}

    	else
    		/* Button Reset */
    		ButtonPress = 0;
    }
    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}
