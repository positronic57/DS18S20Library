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
