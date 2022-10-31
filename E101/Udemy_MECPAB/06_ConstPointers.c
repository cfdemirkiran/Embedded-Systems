/*
 * file name : 06_constPointers.c
 * 
 * Defining const Pointers
 * 
 * Date   : 2022.10.10
 * Author : Cem Furkan Demirkıran
 */


/* Case 1 :
 * 
 * uint8_t const data = 50;
 * using for pi or some spesific datas.
 * 
 * 
 * Case 2 :
 * 
 * uint8_t const *pData = (uint8_t*) 0x40000;
 * here you can modifi pData but data pointed by pdata
 * can not be modifiable.
 * pData is a pointer, pointed the const data.
 * 
 * You can pData = (uint8_t*)0x50000;
 * 
 * 
 * Case 3 :
 * 
 * uint8_t *const pData = (uint8_t*) 0x40000;
 * here you can modify Data but the pointer is can not
 * be modifiable.
 * 
 * You can *pData = 50;
 * 
 * 
 * Case 4 :
 * 
 * uint8_t const const* pData = (uint8_t*) 0x40000;
 * here is pointer and Data are read only.
 * */