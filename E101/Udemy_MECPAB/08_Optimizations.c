/*
 * file name : 08_Optimization.c
 * 
 * Defination : Optimizations 
 * 
 * Date   : 2022.10.10
 * Author : Cem Furkan Demirkıran
 */



#include <stdioh.>
#include <stdint.h>

int main(void){

    uint8_t volatile data1;
    uint8_t volatile data2;

    data1 = 50;

/* here the complire will compile nothing. Because we 
 * create some variables but we wont use in anywhere
 * this is why we have to use "voletile" for do not
 * do any optimization on data1 and data2 operations
 */

/* using O0 is okay but if we use O1++ optimization
 * we have to use volatile.
 */

    data2 = data1;
    data2 = data1;


}
/* GCC has some optimizations these are 
 * -O0 -O1 -O2 -O3
 * -O0 is no optimization
 * -O1 is modarate optimization for memory time.
 * -O2 is full optimization. not debugging friendly.
 * -O3 is like O2 and more aggressive. May cause bugs.
 */

/* Settings -> MCU GCC Comp. -> Optimiz. -> OPT. Level here for access.*/   

