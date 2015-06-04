/*
 * UART.h
 *
 * Copyright (c) 2012  Goce Boshkovski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 */


#include <stdlib.h>
#include <avr/io.h>
#include <avr/iom32.h>

// USART Init
extern void USART_init(unsigned int);
// Send a character via serial port
extern void USART_SendChar(unsigned char);
// Send a string via serial port
extern void USART_SendString(char *);
// Convert an integer value to HEX character and send it via serial port
extern void USART_sendHex(uint8_t);
