/*
 * project name : 07_UART_DataSend
 * file name    : main.c
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.12.12
 *
 * description  : Connected your board to the PC using UART
 *   protocol and created a data sender. Board is creating a
 *   data and sending to the PC with UART protocol. Also
 *   created an character counter for helping the sender.
 *   Created an initialization routine for UART.
 */

#include "bsp.h"

int main(void) {

	BSP_system_init();

	// trap
	for(;;){
		print("HELLO WORLD EVERYONE\n\r");

	}

	return 0;
}
