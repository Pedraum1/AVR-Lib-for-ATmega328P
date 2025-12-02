#ifndef ADC_H

#define ADC_H

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint8_t adc_conversion_complete;

void adc_init();
uint16_t read_analog(uint8_t portc_pin);

#endif