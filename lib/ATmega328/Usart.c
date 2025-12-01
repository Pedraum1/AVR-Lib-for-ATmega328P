#include "Usart.h"

volatile uint8_t usart_run = 1;
volatile char rx_buffer[SIZE_BUFF] = {0x00};
volatile uint16_t rx_buff = 0;

ISR(USART_TX_vect)
{
	usart_run = 1;
}

ISR(USART_RX_vect)
{
	static uint16_t cnt_buff = 0;
	
	rx_buffer[cnt_buff] = UDR0;
	rx_buff++;
	cnt_buff++;
	
	if(cnt_buff >= SIZE_BUFF)	cnt_buff = 0;
}

void usart_init(uint16_t baud_rate, uint8_t fast_mode)
{
	if(fast_mode)
	{
		UCSR0A |= (1<<U2X0);
		UBRR0 = F_CPU/(8UL*baud_rate)-1;
	} else {
		UCSR0A &= ~(1<<U2X0);
		UBRR0 = F_CPU/(16UL*baud_rate)-1;
	}
	
	UCSR0B |=	(1<<RXCIE0) |
				(1<<TXCIE0) |
				(1<<RXEN0)  |
				(1<<TXEN0);
	
	UCSR0C = 0x06;
	sei();
}

void usart_write(char message_byte)
{
	while(!usart_run);
	
	usart_run = 0;
	
	UDR0 = message_byte;
}

void usart_print(char *message)
{
	for(int i = 0; message[i] != '\0' ; i++) usart_write(message[i]);
}

void usart_println(char *message)
{
    for(int i = 0; message[i] != '\0' ; i++) usart_write(message[i]);
	usart_write('\n');
}

void usart_print_number(int num)
{
	char buffer[10];
	int i = 0;

	if (num == 0) {
		usart_write('0');
		return;
	}

	if (num < 0) {
		usart_write('-');
		num = -num;
	}


	while (num > 0) {
		buffer[i++] = (num % 10) + '0';
		num /= 10;
	}

	for (int j = i - 1; j >= 0; j--) {
		usart_write(buffer[j]);
	}
}

uint16_t usart_count()
{
	return rx_buff;
}

char usart_read()
{
	static uint16_t buffer_local = 0;
	char byte_rx = 0;
	byte_rx = rx_buffer[buffer_local];
	buffer_local++;
	rx_buff--;
	
	if(buffer_local>=SIZE_BUFF)	buffer_local = 0;
	
	return byte_rx;
}

void usart_read_string(char *buff)
{
	uint16_t i = 0;

	// Blocking read: wait until we receive a terminator (CR or LF)
	// and collect all bytes that come before it. This prevents
	// partial reads when the incoming line arrives in multiple chunks.
	while (1)
	{
		// wait for at least one byte
		while (usart_count() == 0) ;

		char c = usart_read();

		// treat CR or LF as end of line
		if (c == '\n' || c == '\r')
		{
			break;
		}

		buff[i++] = c;
		if (i >= (SIZE_BUFF - 1))
		{
			break;
		}
	}

	// Null-terminate the string (may be empty)
	buff[i] = '\0';
}
