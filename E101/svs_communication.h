
/*****************************************************************/ 
/* UART COMMAND SERVER						 */	
/*****************************************************************/

/****************************************************************** 
Company		: DEVNET - SVS GROUP
Project		: SVS COMMUNICATION PROTOCOL	
Author		: MFO - DEVNET - SVS GROUP
Start Date	: 07/04/2012

Revision	: 22/07/2014 - Indentation format arrangement etc. 
Place		: SVS TELEKOM - teknopark istanbul
Developer	: MFO - SVS TELEKOM

Revision	: 27/08/2014 - Handler added, multi client can use library. 
Place		: SVS TELEKOM - teknopark istanbul
Developer	: MFO - SVS TELEKOM
******************************************************************/

/************************ PROTOCOL STRUCTURE ********************** 
HEADER			(1 BYTE)
DEVICE ID		(1 BYTE)
RS485 ADDRESS	(1 BYTE)
COMMAND ID		(1 BYTE)
PAYLOAD SIZE	(2 BYTES)
PAYLOAD			(N BYTE(S))
CHECKSUM		(2 BYTES)
******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SVS_COMMUNICATION_H_
#define _SVS_COMMUNICATION_H_

#include <stdint.h>
#include <stddef.h>
#include "stm32f0xx_usart.h"


#define BUFFER_LIMIT			400	// bytes

struct com_handler_t {
	/* Start of Header, Device ID, RS485 Adress, */
	uint8_t tx_com_header, rx_com_header, device_id, rs485_adr;	
	uint8_t common_rs485_address;
	void (*putnc)(const uint8_t *data, size_t size);
	/* Ýletiþimi ledler ile göstermek amacý ile kullanýlan fonksiyon pointerlarý. 
	   Kullanýlmaz ise NULL atanýr. */
	void (*rx_blinker)(void);
	void (*rx_blink_off)(void);	
	void (*tx_blinker)(void);
	void (*tx_blink_off)(void);
	/* RS485 De kontrolu için kullanýlan fonksiyon pointerlarý 
	   Kullanýlmaz ise NULL atanýr. */
	void (*rs485_tx_en)(_Bool enable);
	/* Ýletiþimde kullanýlan checksum */
	uint16_t(*checksum_func)(uint16_t init_val, const uint8_t *data, uint16_t size);
	uint8_t rx_buffer[BUFFER_LIMIT + 10];
	_Bool message_ready;
	uint8_t message_command_id;
	uint16_t message_data_size;
	uint16_t rx_buffer_index;
	int16_t message_discard_timeout_cnt;
	uint16_t MESSAGE_DISCARD_TIMEOUT;
	/* Master cihazlarda, slaveden gelen cevap içerisinde slavein rs485 adresi 
	kontrol edilmek istenmez ise bu deðiþken sýfýr false (0) yapýlýr. 
	Bu deðiþken slave cihazlarda her zaman true (1) yapýlmalýdýr. */
	_Bool check_slave_rs485_address;
};

/* call this function in system timer with 10 ms overflow */ 
void svs_communication_system_timer_function(struct com_handler_t *com_handler);
/* call this function with uart receive byte argument in system uart data receive handler */ 
void svs_communication_parse_message(struct com_handler_t *com_handler, uint8_t data);
/* call this function to check message arrival.
   if message arrives function returns command id and fills data with receive payload, otherwise returns 0. */
uint8_t check_message(struct com_handler_t *com_handler, uint8_t *data);
/* this funtion puts data to uart transmit buffer,
   don't forget to set putc function in communication_template_setup fucntion */
uint16_t checksum_xor(uint16_t init_val, const uint8_t *data, uint16_t size);
uint16_t checksum_mod256(uint16_t init_val, const uint8_t *data, uint16_t size);
uint16_t checksum_crc(uint16_t init_val, const uint8_t *data, uint16_t size);
void send_message(struct com_handler_t *com_handler, uint8_t command, const uint8_t *data, uint16_t data_size);
size_t set_buffer(struct com_handler_t *com_handler, uint8_t *buffer, uint8_t command, const uint8_t *data, uint16_t data_size);
void set_buffer_test(struct com_handler_t *com_handler);

#endif

#ifdef __cplusplus
}
#endif
