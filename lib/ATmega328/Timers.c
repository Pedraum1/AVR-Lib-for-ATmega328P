#include "Timers.h"

void timer0_set_prescaler(uint8_t prescaler)
{
    TCCR0B &= ~(0b00000111);

    if(prescaler > 5) prescaler = 0;

    TCCR0B |= prescaler;
}

void timer0_set_operation_mode(uint8_t mode)
{
    TCCR0A &= ~(0b00000011);
    if(mode > 3) mode = 0;

    TCCR0A |= mode;
}

void timer0_config_oc0a(uint8_t output_mode)
{
    TCCR0A &= ~(0b11000000);
    if(output_mode > 3) output_mode = 0;

    TCCR0A |= (output_mode << 6);
}

void timer0_config_oc0b(uint8_t output_mode)
{
    TCCR0A &= ~(0b00110000);
    if(output_mode > 3) output_mode = 0;

    TCCR0A |= (output_mode << 4);
}

void timer0_set_ocr0a(uint8_t duty_cicle_8bit)
{
    OCR0A = duty_cicle_8bit;
}

void timer0_set_ocr0b(uint8_t duty_cicle_8bit)
{
    OCR0B = duty_cicle_8bit;
}

void timer0_enable_interrupts()
{
    TIMSK0 = 0b00000001;
    sei();
}