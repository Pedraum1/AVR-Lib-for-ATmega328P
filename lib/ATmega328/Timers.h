#ifndef TIMERS_H

#define TIMERS_H

#include "avr/interrupt.h"

#define NORMAL      0
#define PWM_PHASE   1
#define CTC         2
#define FAST_PWM    3

#define PS1     1
#define PS8     2
#define PS64    3
#define PS256   4
#define PS1024  5

#define NORMAL  0
#define TOGGLE  1
#define CLEAR   2
#define SET     3

void timer0_set_operation_mode(uint8_t mode);
void timer0_set_prescaler(uint8_t prescaler);

void timer0_config_oc0a(uint8_t output_mode);
void timer0_config_oc0b(uint8_t output_mode);

void timer0_set_ocr0a(uint8_t duty_cicle_8bit);
void timer0_set_ocr0b(uint8_t duty_cicle_8bit);

void timer0_enable_interrupts();

#endif