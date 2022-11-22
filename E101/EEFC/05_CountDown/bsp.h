/*
 * project name : 05_CountDown
 * file name    : bsp.h
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.11.22
 *
 * description  : Enabling some needed GPIOs and Registers for
 *  main file.
 */

#ifndef BSP_H_
#define BSP_H_

void BSP_ClockEnable(void);
void BSP_SSDEnable(void);
void BSP_ButtonEnable(void);
void BSP_EXITEnable(void);
void BSP_SysTick(void);
void BSP_TIM1(void);
void BSP_TIM3(void);

void BSP_setNumber(int number);


#endif /* BSP_H_ */
