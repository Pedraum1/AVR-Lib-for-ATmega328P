#ifndef STRINGS_H
#define STRINGS_H

#include "avr/io.h"
#include "Boolean.h"

uint8_t string_compare(const char *str1, const char *str2);
uint8_t string_starts_with(const char *str, const char *prefix);

#endif