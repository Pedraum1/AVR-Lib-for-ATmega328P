#ifndef BITS_H
#define BITS_H

#define set_bit(reg, bit)   (reg |=  (1 << bit))
/*  Configura como 1 um bit em específico de um registrador

    Exemplo:
    DDRD = 0b00000000
    set_bit(DDRD, PD0);
    DDRD = 0b00000001
*/

#define clr_bit(reg, bit)   (reg &= ~(1 << bit))
/*  Configura como 0 um bit em específico de um registrador

    Exemplo:
    DDRD = 0b00000001
    ckr_bit(DDRD, PD0);
    DDRD = 0b00000000
*/

#define inp_bit(reg, bit)   (reg &   (1 << bit))
/*  Analisa um bit de um registrador e retorna se este é 0 ou 1

    Exemplo:
    PIND = 0b00000001
    inp_bit(DDRD, PD0); -> true
    inp_bit(DDRD, PD4); -> false
*/

#endif