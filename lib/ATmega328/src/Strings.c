#include "Strings.h"

// Faz a comparação entre duas strings (conjunto de caractéres) se são iguais
uint8_t string_compare(const char *str1, const char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2)
		{
			return false;
		}
		str1++;
		str2++;
	}

	return (*str1 == '\0' && *str2 == '\0') ? true : false;
}

// Verifica se uma string inicia com um determinado prefixo
uint8_t string_starts_with(const char *str, const char *prefix)
{
	while(*prefix != '\0')
	{
		if(*str == '\0') return false;	//str acabou antes
		if(*str != *prefix) return false; //char diferente

		str++;
		prefix++;
	}

	return true;
}