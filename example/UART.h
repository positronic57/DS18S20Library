/*
 * UART.h
 *
 * Created: 13-Jul-15 23:49:17
 * Author: Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 */ 

#include <stdlib.h>
#include <avr/io.h>

//USART init
void USART_init(unsigned int);

//Send one char via USART interface
void USART_SendChar(unsigned char);

//Send a string via USART interface
void USART_SendString(char *);

//Convert unsigned 8 bit integer value to a HEX string and sends it via USART interface
void USART_sendHex(uint8_t);
