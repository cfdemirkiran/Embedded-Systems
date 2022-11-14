
//==================================================================
/* svs_communication.c
   SVS TELEKOM'a ait protokol iletiþim handling modülü. 
   Bu modülü birden fazla client kullanabilir. 
   struct com_handler_t türünden bir handle deðiþkeni oluþturularak, ilgili client kendine ait
   handle deðiþkeni ile fonksiyon ve veri elemanlarýný kullanýr. */
//==================================================================
#include "svs_communication.h"
#include <string.h>
//==================================================================
/* call this function in system timer with 10 ms overflow */
void svs_communication_system_timer_function(struct com_handler_t *com_handler)
{
	if (com_handler->message_discard_timeout_cnt)
		com_handler->message_discard_timeout_cnt--;
}
//==================================================================
/* Gelen datayý protokole göre okuyan fonksiyon.
   Genelde uart interrupt içerisinde çaðrýlýr. */
void svs_communication_parse_message(struct com_handler_t *com_handler, uint8_t usart2_data)
{	
	/*
			USART_SendData(USART2, data);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET || !USART_GetFlagStatus(USART2, USART_FLAG_TC))
			;
	*/
	if (com_handler->rx_blinker != NULL)
		com_handler->rx_blinker();
	/* don't receive new message until rx_buffer is empty */
	if (com_handler->message_ready) 
		return;
	if (com_handler->message_discard_timeout_cnt == -1)
		com_handler->rx_buffer_index = 0;
	/* message was not completed in MESSAGE_DISCARD_TIMEOUT, receive timeout occured. reset commumication. */
	if (com_handler->rx_buffer_index > 0 && com_handler->message_discard_timeout_cnt == 0)
		goto RESET; 
	/* buffer bound check	*/
	if (com_handler->rx_buffer_index >= BUFFER_LIMIT) 
		goto RESET;
	/* set timeout counter	*/
	com_handler->message_discard_timeout_cnt = com_handler->MESSAGE_DISCARD_TIMEOUT; 
	/* fill receive buffer	*/
	com_handler->rx_buffer[com_handler->rx_buffer_index] = usart2_data; 
	if (com_handler->rx_buffer_index == 0) {
		/* header check	*/
		if (com_handler->rx_buffer[0] != com_handler->rx_com_header) 
			goto RESET;
	} else if (com_handler->rx_buffer_index == 1) {
		/* device_id check	*/
		if (com_handler->rx_buffer[1] != com_handler->device_id) 
			goto RESET;
	} else if (com_handler->rx_buffer_index == 2) {	
		/* 2 tane RS485 adres var ise, ikisini de kontrol et. */
		if (com_handler->check_slave_rs485_address && 
		com_handler->rx_buffer[2] != com_handler->rs485_adr && com_handler->rx_buffer[2] != com_handler->common_rs485_address)
			goto RESET;
	} else if (com_handler->rx_buffer_index == 5) {
		com_handler->message_command_id = com_handler->rx_buffer[3];
		/* get payload size	*/
		memcpy(&com_handler->message_data_size, com_handler->rx_buffer + 4, sizeof(com_handler->message_data_size));	
		if (com_handler->message_data_size >= BUFFER_LIMIT)
			goto RESET;
	} else if (com_handler->rx_buffer_index == com_handler->message_data_size + 7) {
		if (com_handler->rx_blink_off != NULL)
			com_handler->rx_blink_off();
		/* data ready	*/
		com_handler->message_ready = 1; 
		com_handler->rx_buffer_index = 0;
		return;
	}
	/* increment buffer index	*/
	com_handler->rx_buffer_index++; 
	return;
RESET:
	com_handler->message_discard_timeout_cnt = -1;
	com_handler->rx_buffer_index = 0;
}
//==================================================================
/* call this function to check message arrival.
   if message arrives function returns command id and fills data with receive payload, otherwise returns 0. 
   Bu fonksiyon bir sorgunun gelip gelmediðini kontrol etmek için periyodik olarak çaðrýlmalýdýr. 
   Eðer bir sorgu gelir ise modül, uint8_t *data deðikenini gelen payload ile doldurur. */
uint8_t check_message(struct com_handler_t *com_handler, uint8_t *data)
{
	uint8_t command_id;
	uint16_t checksum;
	
	if (!com_handler->message_ready) /* message arrived ? */
		return 0;
	/* checksum control */
	memcpy(&checksum, com_handler->rx_buffer + com_handler->message_data_size + 6, sizeof(checksum));
	if (com_handler->checksum_func(0, com_handler->rx_buffer, com_handler->message_data_size + 6) != checksum)
		goto RESET;
	memcpy(data, com_handler->rx_buffer + 6, com_handler->message_data_size);
	command_id = com_handler->message_command_id;
	com_handler->message_ready = 0;
	
	return command_id;
RESET:
	com_handler->message_ready = 0;
	return 0;
}
//=======================================================================
/* XOR hata kontrol fonksiyonu */
uint16_t checksum_xor(uint16_t init_val, const uint8_t *data, uint16_t size)
{
	uint16_t i;
	uint8_t checksum = init_val;

	for (i = 1, checksum = data[0]; i <= size; i++)
		checksum ^= data[i];

	return checksum;
}
//==================================================================
/* MOD256 hata kontrol fonksiyonu */
uint16_t checksum_mod256(uint16_t init_val, const uint8_t *data, uint16_t size)
{
	uint16_t i;
	uint16_t sum = init_val;

	if (size > BUFFER_LIMIT)
		return 0;

	for (i = 0; i < size; i++)
		sum += data[i];

	return (uint8_t) (sum % 256);
}
//=======================================================================
/* CRC Kermit hata kontrol fonksiyonu 
   Renesas M16C ye ait donanýmsal CRC yapýsý ile ayný sonucu üretir. */
uint16_t checksum_crc(uint16_t init_val, const uint8_t *data, uint16_t size)
{
	uint16_t crc_buff = init_val;
	uint16_t j;
	uint8_t i, input;

	for (j = 0; j < size; j++)
		for (i = 0, input = *data++; i < 8; i++) {
			crc_buff = crc_buff >> 1 ^ (uint16_t) (((crc_buff & 0x0001) ^ (input & 0x01)) ? 0x8408 : 0x0000);
			input >>= 1;
		}
	return crc_buff;
}
//=======================================================================
static uint16_t putnc_and_checksum_wrapper(struct com_handler_t *com_handler, const uint8_t *data, size_t size, uint16_t checksum_init_val)
{
	if (com_handler->tx_blinker != NULL)
		com_handler->tx_blinker();
	
	com_handler->putnc(data, size);
	
	if (com_handler->tx_blink_off != NULL)
		com_handler->tx_blink_off();
	
	return com_handler->checksum_func(checksum_init_val, data, size);
}
//=======================================================================
/* Sorgu veya cevap göndermek için kullanýlan fonksiyondur. */
void send_message(struct com_handler_t *com_handler, uint8_t command, const uint8_t *data, uint16_t data_size)
{
	uint16_t checksum;

	if (com_handler->rs485_tx_en != NULL)
		com_handler->rs485_tx_en(1);
	
	checksum = putnc_and_checksum_wrapper(com_handler, &com_handler->tx_com_header, sizeof(com_handler->tx_com_header), 0);
	checksum = putnc_and_checksum_wrapper(com_handler, &com_handler->device_id, sizeof(com_handler->device_id), checksum);
	checksum = putnc_and_checksum_wrapper(com_handler, &com_handler->rs485_adr, sizeof(com_handler->rs485_adr), checksum);
	checksum = putnc_and_checksum_wrapper(com_handler, &command, sizeof(command), checksum);
	checksum = putnc_and_checksum_wrapper(com_handler, (uint8_t *)&data_size, sizeof(data_size), checksum);
	
	if (data != NULL && data_size > 0) {
		checksum = putnc_and_checksum_wrapper(com_handler, data, data_size, checksum);
	}
	checksum = putnc_and_checksum_wrapper(com_handler, (uint8_t *)&checksum, sizeof(checksum), checksum);
	
	if (com_handler->rs485_tx_en != NULL)
		com_handler->rs485_tx_en(0);
}
//=======================================================================
static uint16_t memcpy_and_checksum_wrapper(struct com_handler_t *com_handler, uint8_t **buffer, const uint8_t *data, uint16_t data_size, uint16_t checksum_init_val)
{
	memcpy(*buffer, data, data_size);
	*buffer = *buffer + data_size;
	return com_handler->checksum_func(checksum_init_val, data, data_size);
}
//=======================================================================
/* Gönderilecek veri paketini hazýrlayan fonksiyondur. 
   Veri gönderimi yapmaz. Ýsteðe baðlý olarak kullanýlabilir. 
   Normalde sorgu göndermek için "send_message" fonksiyonu yeterlidir. */  
size_t set_buffer(struct com_handler_t *com_handler, uint8_t *buffer, uint8_t command, const uint8_t *data, uint16_t data_size)
{
	uint16_t checksum;

	checksum = memcpy_and_checksum_wrapper(com_handler, &buffer, &com_handler->tx_com_header, sizeof(com_handler->tx_com_header), 0);
	checksum = memcpy_and_checksum_wrapper(com_handler, &buffer, &com_handler->device_id, sizeof(com_handler->device_id), checksum);
	checksum = memcpy_and_checksum_wrapper(com_handler, &buffer, &com_handler->rs485_adr, sizeof(com_handler->rs485_adr), checksum);
	checksum = memcpy_and_checksum_wrapper(com_handler, &buffer, &command, sizeof(command), checksum);	
	checksum = memcpy_and_checksum_wrapper(com_handler, &buffer, (uint8_t *)&data_size, sizeof(data_size), checksum);
	
	if (data != NULL && data_size > 0) {
		checksum = memcpy_and_checksum_wrapper(com_handler, &buffer, data, data_size, checksum);
	}
	memcpy(buffer, &checksum, sizeof(checksum));

	return data_size + 8;
}
//=======================================================================
void set_buffer_test(struct com_handler_t *com_handler)
{
	uint8_t buffer[100];
	
	set_buffer(com_handler, buffer, 1, NULL, 0);
}
//=======================================================================

