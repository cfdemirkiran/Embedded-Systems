/*
 * project name : 06_UART_Echo
 * file name    : main.c
 *
 * author       : Cem Furkan Demirkıran
 *
 * date         : 2022.12.06
 *
 * description  : Connected your board to the PC using UART
 *   protocol and making an Echo. Whenever user pressing a key
 *   from PC, board will Echo and sendback the data. Created
 *   an initialization routine for UART.
 */

#include "bsp.h"

int main(void) {

	BSP_system_init();
	BSP_UART_init(9600);

	// trap
	for(;;){
	}

	return 0;
}
