/*
 * file name : 10_Padding.c
 * 
 * Defination : Aligned and unaligned Memory consume on Structures
 * 
 * Date   : 2022.10.11
 * Author : Cem Furkan Demirkıran
 */



/* When you make aligned memory, your structure
 * variables will make a line for consuming
 * memory. When you make this, you can use more
 * memory consuming but get more speed.
 */

/* When you use padding "aligned" assamly part 
 * use only one str for values but if you donot
 * padding, then code have to use lots of strb
 * so the code lines will increase and optimisition
 * will be getting worse. 
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

struct DataSet2
{
    char  data5;
    int   data6;
    char  data7;
    short data8;
}__attribute__((packed)); // if you do this.

int main(void){

    struct DataSet data;
    struct DataSet2 data2;

    data.data1  = 0x11;
    data.data2  = 0xFFFFEEEE;
    data.data3  = 0x22;
    data.data4  = 0xABCD;

    data2.data5  = 0x11;
    data2.data6  = 0xFFFFEEEE;
    data2.data7  = 0x22;
    data2.data8  = 0xABCD;

    uint8_t *ptr;

    ptr = (uint8_t*)&data;

    uint64_t totalSize1 = sizeof(struct DataSet);
    uint64_t totalSize2 = sizeof(struct DataSet2);

    for(uint32_t i = 0; i < totalSize1 ; i++)
    {
        printf("%p       %X\n",ptr, *ptr);
        ptr++;
    }

    printf("Total memory consumed = %I64u\n ",sizeof(struct DataSet));
    printf("\n\n");

        for(uint32_t i = 0; i < totalSize2 ; i++)
    {
        printf("%p       %X\n",ptr, *ptr);
        ptr++;
    }
    
    
    printf("Total memory consumed = %I64u\n ",sizeof(struct DataSet2));
}
