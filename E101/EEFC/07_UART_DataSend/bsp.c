/*
 * project name : 07_UART_DataSend
 * file name    : bsp.c
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.12.12
 *
 * description  : Board Support Package is created for UART
 *   and delay functions. Registers and EXITs are created in
 *   this file.
 */

#include "bsp.h"

#define BSP_GREEN_LED_PIN (6U)
#define BSP_BUTTON_PIN (2U)

static volatile uint32_t tick = 0;
uint8_t receive_data = 0;

void BSP_system_init(){

	__disable_irq();
	//SystemCoreClockUpdate();
	//BSP_led_init();

	SysTick_Config(SystemCoreClock / 1000);
	BSP_UART_init(9600);
	__enable_irq();


	//USART2_IRQHandler();
	//BSP_IWDG_init();

}

void SysTick_Handler(void){

	if(tick > 0){
		tick;
	}
}

void delay_ms(uint32_t s){
	tick = s;
	while(tick);
}

void delay(volatile unsigned int s){
	for(; s>0; s--);
}

void BSP_IWDG_init(void){
	IWDG -> KR = 0x5555;
	IWDG -> PR = 1;
	while(IWDG -> SR & 0x01);

	IWDG -> KR = 0xCCCC;
}

void BSP_IWDG_feed(void){
	IWDG->KR = 0xAAAA;
}

void USART2_IRQHandler(void){

	uint8_t data = (uint8_t) USART2 -> RDR;
	// RXNE is automatically cleared when read
	//receive_data = data;
	printChar(data);
}

void printChar(uint8_t c){

	USART2 -> TDR = (uint16_t) c;
	while(!(USART2 -> ISR & (1 << 6)));
}

void _print(int fd, char *buf, int len){

	(void)fd;
	for(int i=0; i<len; ++i){
		printChar(buf[i]);
	}
}

void print(char *buf){
	int len = 0;
	while(buf[len++] != '\0');
	_print(0, buf, len);
}

/* PA2 PA3 are connected to the Virtual COM port.
 * USART2 module
 */
void BSP_UART_init(uint32_t baud){
	RCC -> IOPENR  |= (1U << 0);
	RCC -> APBENR1 |= (1U << 17);

	// setup PA2 as AF1
	GPIOA -> MODER  &=  ~(3U << 2*2);
	GPIOA -> MODER  |=  (2U << 2*2);
	// choose AF1 from MUX
	GPIOA -> AFR[0] &=  ~(0xFU << 4*2);
	GPIOA -> AFR[0] |=  (1U << 4*2);

	// setup as PA3 as AF1
	GPIOA -> MODER &= ~(3U << 2*3);
	GPIOA -> MODER |= (2U  << 2*3);
	//choose AF1 from MUX
	GPIOA -> AFR[0] &=  ~(0xFU << 4*3);
	GPIOA -> AFR[0] |= (1U << 4*3);

	// setup USART2
	// reset USART CR1
	USART2 -> CR1 = 0;
	USART2 -> CR1 |= (1 << 3); // TE
	USART2 -> CR1 |= (1 << 2); // RE
	USART2 -> CR1 |= (1 << 5); //RXNEINE

	USART2 -> BRR = (uint16_t)(SystemCoreClock / baud); // baud rate register

	// enable USART2
	USART2 -> CR1 |= (1 << 0); // UE

	// setup NVIC
	NVIC_SetPriority(USART2_IRQn, 1);
	NVIC_EnableIRQ(USART2_IRQn);
}

void BSP_ClockEnable(){
    /* Enable GPIOA-B clock */
    RCC->IOPENR |= (1U << 0);
    RCC->IOPENR |= (1U << 1);
}

void BSP_SysTick(){
	/* System Clock */
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 500);
}
