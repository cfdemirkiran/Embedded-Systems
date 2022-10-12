/*
 * file name : 14_Arrays.c
 * 
 * Defination : Understanding Arrays
 * 
 * Date   : 2022.10.11
 * Author : Cem Furkan Demirkıran
 */



#include <stdint.h>
#include <stdio.h>

/* Syntax :
 * uint8_t MyArray1 [value] = {ver1, ver2, ver3, ...}
 * You can make value empty but if you make empty array
 * you have to give some veriables. Also you can make
 * your value a veriable but you have to defie before the
 * array. int a = 10; int array[a] ; like that.
 * also if you dont write veriables, the empty veriables
 * are ll be zeros.
 */

int main(void){

    uint8_t ArrayLen = 10;
    uint8_t MyArray[ArrayLen] = {1,2,3,4,5,6,7,8,9};

    while(0>=ArrayLen)
    {
        printf(" %x\t",*(MyArray[ArrayLen]));
        ArrayLen--;
    }

    

    return 0;
}