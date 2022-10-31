/*
 * file name : 07_Ex_ReadLed.c
 * 
 * Reading data from pin and 
 * 
 * Date   : 2022.10.10
 * Author : Cem Furkan Demirkıran
 */

#include<stdint.h>

int main(void)
{
	uint32_t *pClkCtrlReg =   (uint32_t*)0x40023830;
	uint32_t *pPortDModeReg = (uint32_t*)0x40020C00;
	uint32_t *pPortDOutReg =  (uint32_t*)0x40020C14;

	uint32_t *pPortAModeReg = (uint32_t*)0x40020C00;
    uint32_t *pPortAInReg =   (uint32_t*)0x40020C10;

	//1. enable the clock for GPOID, GPIOA peripheral in the AHB1ENR
	*pClkCtrlReg |= ( 1 << 3);
    *pClkCtrlReg |= ( 1 << 0);

	// configuring PD12 as output
	*pPortDModeReg &= ~( 3 << 24);
	//b. make 24th bit position as 1 (SET)
	*pPortDModeReg |= ( 1 << 24);

    //Configure PA0 as input mode (GPIOA MODE REGISTER)
    *pPortAModeReg &= ~(3 << 0);
while(1){
    //read the pin status of the pin PA0 (GPIOA INPUT DATA REGISTER)
    uint8_t PinStatus = (*pPortAInReg & 0x1);

    if(PinStatus){
        //turn on the LED
        *pPortDOutReg |= ( 1 << 12);
    }
    else{
        //turn off the LED
        *pPortDOutReg &= ~( 1 << 12);

    }

}
}