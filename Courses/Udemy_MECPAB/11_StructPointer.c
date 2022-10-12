/*
 * file name : 11_StructPointer.c
 * 
 * Defination : Using struct pointer
 * 
 * Date   : 2022.10.11
 * Author : Cem Furkan Demirkıran
 */



#include <stdint.h>
#include <stdio.h>

struct DataSet
{
    char  data1;
    int   data2;
    char  data3;
    short data4;
};

int main(void)
{

    struct DataSet data;

    data.data1 = 0x11;
    data.data2 = 0xFFFFEEEE;
    data.data3 = 0x22;
    data.data4 = 0xABCD;

    printf("Before : data.data1 = %X\n", data.data1);

    struct DataSet *pData;

    pData = &data;
    pData -> data1 = 0x55;

    printf("After : data.data1 = %X\n", data.data1);

    getchar();
    return 0;
}