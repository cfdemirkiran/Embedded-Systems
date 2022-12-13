/*
 * project name : 06_UART_DataSend
 * file name    : bsp.h
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.12.12
 *
 * description  : Board Support Package's header files are
 *   storing here.
 */

#ifndef BSP_H_
#define BSP_H_

#include "stm32g0xx.h"

/* Common API function for Nucleo STM32G031 board */

void BSP_system_init();

void BSP_IWDG_init(void);
void BSP_IWDG_feed(void);

void delay_ms(uint32_t);
void delay(volatile unsigned int);

void BSP_UART_init(uint32_t);
void printChar(uint8_t);
void _print(int , char *, int);
void print(char *);

void BSP_system_init(void);
void BSP_ClockEnable(void);
void BSP_SysTick(void);

#endif /* BSP_H_ */
