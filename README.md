# ATmega328P — AVR Helper Library

Uma coleção leve de utilitários e drivers para AVR ATmega328P, organizada como uma pequena biblioteca C para uso com PlatformIO/AVR-GCC.

**Propósito:** facilitar operações comuns (manipulação de bits, ADC, timers, etc.) em projetos embarcados com ATmega328P, mas sem apelar
para a clássico "Arduino.h". Essa biblioteca mantém o usuário próximo do nível do Hardware sem perder as facilidades de uma biblioteca em C
que abstrai todas as operações de bitwise.

###  Estrutura do repositório
- **`lib/ATmega328/`**: implementações da biblioteca
  - `Adc.h`, `Adc.c` — conversor analógico-digital com suporte a interrupção
  - `Bits.h` — macros para manipulação de bits (`set_bit`, `clr_bit`, `inp_bit`, `toggle_bit`)
  - `Boolean.h` — definições simples `true`/`false`
  - `Timers.h`, `Timers.c` — (timers/servos — consulte os arquivos para detalhes)
  - `ATmega328.h` — header único que inclui os módulos disponíveis
- **`src/`**: exemplo de uso / firmware

### Como usar
- Adicione a pasta da biblioteca ao seu projeto PlatformIO (já está dentro de `lib/`).
- Inclua no seu código principal:

```c
#include <avr/io.h>
#include "ATmega328.h" // inclui todos os módulos da biblioteca
```

Se preferir apenas um módulo específico:

```c
#include "Bits.h"
#include "Adc.h"
```

### Instalação / Build (PlatformIO)
- Certifique-se de ter o PlatformIO instalado.
- Na raiz do projeto (onde está o `platformio.ini`), compile com:

```powershell
pio run
```

ou para upload (quando conectado):

```powershell
pio run --target upload
```

## API e exemplos rápidos

### Macros de manipulação de bits (`Bits.h`)
- `set_bit(reg, bit)` — seta um bit (1) em `reg`.
- `clr_bit(reg, bit)` — limpa um bit (0) em `reg`.
- `inp_bit(reg, bit)` — retorna o valor do bit (teste).
- `toggle_bit(reg, bit)` — inverte o bit.

Exemplo:

```c
DDRD = 0x00; // porta D como entrada
set_bit(DDRD, PD0); // agora PD0 é saída
toggle_bit(PORTD, PD0); // inverte nível no pino PD0
if (inp_bit(PIND, PD0)) { /* pino em nível alto */ }
```

### Boolean (`Boolean.h`)
- Definições simples: `true` = 1, `false` = 0.
Use quando precisar de um booleano leve sem depender de stdbool.h.

### Conversor AD (`Adc.h` / `Adc.c`)
- `void adc_init()`
  - Inicializa o ADC: referência em AVcc, habilita interrupção e seleciona prescaler (divisão por 128).
- `uint16_t read_analog(uint8_t portc_pin)`
  - Lê um canal analógico do `PORTC` (PC0..PC5). Retorna 0 se `portc_pin > 5`.
  - Implementação: desabilita buffer digital no canal, seleciona canal no `ADMUX`, inicia conversão e aguarda a flag `adc_conversion_complete` populada pela ISR `ADC_vect`.

Exemplo de uso:

```c
#include "Adc.h"

int main(void)
{
    adc_init();
    uint16_t val;

    while (true)
    {
        val = read_analog(0); // lê PC0
        // use 'val' (0..1023) conforme necessário
    }

    return 0;
}
```

Observação: a biblioteca define `volatile uint8_t adc_conversion_complete` e usa a ISR `ADC_vect` para sinalizar fim de conversão.

### Timers
- `void timer0_set_operation_mode(uint8_t mode)` — configura o modo de operação (NORMAL, PWM_PHASE, CTC, FAST_PWM).
- `void timer0_set_prescaler(uint8_t prescaler)` — define o divisor de frequência (PS1, PS8, PS64, PS256, PS1024).
- `void timer0_config_oc0a(uint8_t output_mode)` — configura o modo de comparação para OC0A (NORMAL, TOGGLE, CLEAR, SET).
- `void timer0_config_oc0b(uint8_t output_mode)` — configura o modo de comparação para OC0B.
- `void timer0_set_ocr0a(uint8_t duty_cicle_8bit)` — define o valor de comparação (ciclo de trabalho) para OC0A.
- `void timer0_set_ocr0b(uint8_t duty_cicle_8bit)` — define o valor de comparação para OC0B.
- `void timer0_enable_interrupts()` — habilita interrupções do Timer0.

Exemplo de uso (PWM em OC0A):
```c
timer0_set_operation_mode(FAST_PWM);    // modo PWM rápido
timer0_set_prescaler(PS64);              // prescaler = 64
timer0_config_oc0a(CLEAR);               // modo não-invertido
timer0_set_ocr0a(128);                   // 50% de ciclo (0..255)
timer0_enable_interrupts();
```

É necessário que o usuário , ao utilizar os timers, desenvolva as funções de interrupção dentro de uma ISR() própria do código desenvolvido.
Essa escolha de design visa evitar conflitos com interrupções da biblioteca. Por este mesmo motivo não há funções como millis() ou micros() que
dependem dos timers do hardware para funcionar.

### USART (Serial)
- `void usart_init(uint16_t baud_rate, uint8_t fast_mode)` — inicializa a comunicação serial na taxa de bauds especificada. Use `fast_mode=1` para modo U2X (útil para 9600 baud com clock 16MHz).
- `void usart_write(char message_byte)` — escreve um caractere no barramento serial.
- `void usart_print(char *message)` — escreve uma string no barramento serial.
- `void usart_println(char *message)` — escreve uma string com quebra de linha (`\n`).
- `void usart_print_number(int num)` — escreve um número inteiro no barramento serial (suporta negativos).
- `char usart_read()` — lê um caractere do buffer de recepção.
- `void usart_read_string(char *buff)` — lê uma string até encontrar `\n` ou `\r e guarda numa variável fornecida.
- `uint8_t usart_available()` — verifica se há dados disponíveis no buffer de recepção.
- `uint16_t usart_count()` — retorna a quantidade de bytes no buffer de recepção.
- `uint8_t string_compare(const char *str1, const char *str2)` — compara duas strings (retorna `true` se iguais).

Exemplo de uso:
```c
#include "ATmega328.h"

int main(void)
{
    usart_init(9600, 1);  // 9600 baud, modo rápido
    
    usart_println("Iniciando...");
    
    while (true)
    {
        if (usart_available()) {
            char c = usart_read();
            usart_write(c);  // echo
        }
    }

    return 0;
}
```

Também é possível utilizar a biblioteca para leitura de strings:

```c
#include "ATmega328.h"

int main(void)
{
    usart_init(9600, 1);  // 9600 baud, modo rápido

    char message[SIZE_BUFFER] = {'\0'};
    
    usart_println("Iniciando...");
    
    while (true) {
        if (usart_available()) {
            usart_read_string(message);
            usart_println(message);  // echo
        }
    }

    return 0;
}
```

Observação: a biblioteca usa interrupções para USART (ISRs `USART_TX_vect` e `USART_RX_vect`) e mantém um buffer circular de recepção com tamanho `SIZE_BUFF` (padrão: 64 bytes).

## Boas práticas
- Evite acessar canais analógicos fora de 0..5 (a função `read_analog` retorna 0 nesses casos).
- Proteja seções críticas se utilizar variáveis compartilhadas entre ISRs e o código principal (use `volatile` e/ou desabilite interrupções quando necessário).

## Autores
- [@Pedraum1](https://github.com/Pedraum1) - Criador e Idealizador