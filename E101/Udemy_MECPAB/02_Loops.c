/*
 * file name : 02_Loops.c
 * 
 * Defining Loops
 * 
 * Date   : 2022.10.10
 * Author : Cem Furkan Demirkıran
 */
#include <stdio.h>
#include <stdint.h>

int main(){

    /* while loop */
    /* repeat until expression becomes false */

     uint8_t num = 1;

     while(num <=10){

        printf("%d\n", num++);
     }
     
   return 0;
}