#include "Adc.h"

ISR(ADC_vect)
{
	adc_conversion_complete = 1;
}

void adc_init()
{
	ADMUX |= (1<<REFS1);	// Configura para usar referência interna no Avcc | ADMUX = 0b01000000
	ADCSRA |= (1<<ADEN)		// Habilita a conversão AD
		   |  (1<<ADIE)		// Habilita interrupção do ADC
		   |  0x07;		    // Configura o fator de divisão para 128
}

uint16_t read_analog(uint8_t portc_pin)
{
    if(portc_pin > 5) return 0;		// retorno para evitar leituras no portc além do PC5
	
	adc_conversion_complete = 0;	// limpa a flag de conversão AD
	
	DIDR0 |= (1<<portc_pin);		// desliga o buffer do canal utilizado
	ADMUX &= 0xF0;					// limpa o nibble menos significativo
	ADMUX |= portc_pin;				// atualiza o canal de conversão AD
	
	ADCSRA |= (1<<ADSC);			// inicia nova conversão

	while(!adc_conversion_complete);// aguarda conversão
	
	return ADC;
}