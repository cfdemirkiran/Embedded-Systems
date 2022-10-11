/*
 * file name : 05_const.c
 * 
 * Defining const type
 * 
 * Date   : 2022.10.10
 * Author : Cem Furkan Demirkıran
 */




/* If you want to make your veriable read only
 * you should use const.
 * uint8_t const data1 = 10;
 * uint8_t data2 = 10 
 * you can change data2 but you can not change data1
 */

/* But if you want to change data1, you have to
 * change the value by address.
 */

#include <stdio.h>
#include <stdint.h>


int main(){

    uint8_t data = 10;
    printf("Value = %u\n",data);

    uint8_t *ptr = (uint8_t*) &data;

    *ptr = 50;
    printf("Value = %u\n",data);

    getchar();
}