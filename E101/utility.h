/*==================================================================================================================================*/
#ifndef _UTILITY_H_
#define _UTILITY_H_
/*==================================================================================================================================*/
#include <stdint.h>
/*==================================================================================================================================*/
/*==================================================================================================================================*/
/* milisecond */
#define TIMER_INTERRUPT_PERIOD	10

#define TIMER_50MS		(50 / TIMER_INTERRUPT_PERIOD)
#define TIMER_200MS		(200 / TIMER_INTERRUPT_PERIOD)
#define TIMER_300MS		(300 / TIMER_INTERRUPT_PERIOD)
#define TIMER_500MS		(500 / TIMER_INTERRUPT_PERIOD)
#define TIMER_1S		(1000 / TIMER_INTERRUPT_PERIOD)
/*==================================================================================================================================*/

void delay_ms(uint16_t msecs);
void delay_us(uint16_t value);
void timer14_init(void);
void timer3_init(void);
void timer6_init(void);
void timer_init(void);
void set_delay_msec(uint16_t msecs);
int is_timeout(_Bool or_flag);
uint8_t checksum(const uint8_t *buf, uint16_t size);
char hex_to_ascii(uint8_t val, _Bool high_low);
uint8_t hexStringToUint8(uint8_t low , uint8_t high);

/*==================================================================================================================================*/
#endif	/* #ifndef _UTILITY_H_	*/
/*==================================================================================================================================*/

