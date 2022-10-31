/*
 * file name : 09_Structures.c
 * 
 * Defination : Understanding Structure data types
 * 
 * Date   : 2022.10.11
 * Author : Cem Furkan Demirkıran
 */



/* Structure is user defined data type.*/
/* Syntax : 
 * struct tag_name
 * {
 *  memebers
 * };
 */

/* Struct doesn't consume any memory. But if you
 * create a structure variable. It ll consume 
 * if you want to calculate memory, you should 
 * calculate from members variables. 
 */

#include <stdint.h>
#include <stdio.h>

struct carModel
{
    uint32_t carNumber;
    uint32_t carPrice;
    uint16_t carMaxSpeed;
    float    carWeigth;
};

int main(void)
{
    /* This is C89 method. Order is important */
    struct carModel carBMW = {2021,15000,220,1330};

    /* This is C99 method. */
    struct carModel carFord = {.carWeigth = 90.90, .carMaxSpeed = 150};

    printf("Details of car BMW is as follows\n");
    printf("carNumber = %u\n",carBMW.carNumber); 

    printf("Sizeof of struct carModel is %I64u\n",sizeof(carBMW));
    printf("Sizeof of struct carModel is %I64u\n",sizeof(carFord));
    return 0;
}