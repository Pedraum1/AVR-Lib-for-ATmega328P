#ifndef USART_T

#define USART_T

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SIZE_BUFF 64

void usart_init(uint16_t baud_rate, uint8_t fast_mode);
void usart_write(char message_byte);
void usart_print(char *message);
void usart_println(char *message);
void usart_print_number(int num);
uint16_t usart_count();
char usart_read();
void usart_read_string(char *buff);

#endif