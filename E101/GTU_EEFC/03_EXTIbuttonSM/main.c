/*
 * project name : 03_EXTIbuttonSM
 * file name    : main.c
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.10.27
 *
 * description   : A state machine blink the external LED at different intervals.
 *   Assigned each speed to a mode, and attach a button to cycle through the modes.
 *   Used external interrupts to detect button presses and used the handler to
 *   change the states. Each button press will cycle through these modes.
 */

#include "stm32g0xx.h"

/* 1 Sec is 1600000 */
void delay(volatile uint32_t);

volatile uint32_t ButtonPress = 0;

/* Interrupt Handlers */
void EXTI0_1_IRQHandler(void){

    if (ButtonPress != 5) {
            delay(50);
            if(ButtonPress !=5)
                ButtonPress ++;
            else
                ButtonPress = 0;
        }
        else {
            ButtonPress = 0;
        }
        delay(100000);
        EXTI->RPR1 |= (1U << 1);
}



int main(void) {

    /* Enable GPIOA clock */
    RCC->IOPENR |= (1U << 0);

    /* Setup PA0 as output */
    GPIOA->MODER &= ~(3U << 2*0);
    GPIOA->MODER |= (1U << 2*0);

    /* Setup PA1 as input */
    GPIOA->MODER &= ~(3U << 2*1);
    GPIOA->MODER |= (0U << 2*1);

    /* External interrupt at PA1 port */
    EXTI->EXTICR[0] |= (0U << 8*1);

    /* Mask and Rising on Px1 */
    EXTI->IMR1  |= (1U << 1);
    EXTI->RTSR1 |= (1U << 1);

    /* Setup NVIC */
    NVIC_SetPriority(EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    while(1) {

        switch(ButtonPress){

            case 0:
                /* Turn off LED */
                GPIOA->ODR = (0U << 0);
                break;

            case 1:
                /* Turn on LED 2sec interval */
                GPIOA->ODR |= (1U << 0);
                delay(3200000);
                GPIOA->ODR ^= (1U << 0);
                delay(3200000);
                break;

            case 2:
                /* Turn on LED 1sec interval */
                GPIOA->ODR |= (1U << 0);
                delay(1600000);
                GPIOA->ODR ^= (1U << 0);
                delay(1600000);
                break;

            case 3:
                /* Turn on LED 0.5sec interval */
                GPIOA->ODR |= (1U << 0);
                delay(800000);
                GPIOA->ODR ^= (1U << 0);
                delay(800000);
                break;

            case 4:
                /* Turn on LED 0.1sec interval */
                GPIOA->ODR |= (1U << 0);
                delay(160000);
                GPIOA->ODR ^= (1U << 0);
                delay(160000);
                break;

            case 5:
                /* Turn on LED */
                GPIOA->ODR |= (1U << 0);
                break;

        }
    }
    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}
