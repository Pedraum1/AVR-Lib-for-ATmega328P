# ATmega328P — AVR Helper Library

Uma coleção leve de utilitários e drivers para AVR ATmega328P, organizada como uma pequena biblioteca C para uso com PlatformIO/AVR-GCC.

**Propósito:** facilitar operações comuns (manipulação de bits, ADC, timers, etc.) em projetos embarcados com ATmega328P.

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

int main(void) {
    adc_init();
    uint16_t val;

    while (1) {
        val = read_analog(0); // lê PC0
        // use 'val' (0..1023) conforme necessário
    }
}
```

Observação: a biblioteca define `volatile uint8_t adc_conversion_complete` e usa a ISR `ADC_vect` para sinalizar fim de conversão.

### Timers
- Os detalhes de timers e utilitários associados estão em `Timers.h` / `Timers.c`. Consulte esses arquivos para APIs específicas (configuração de prescalers, callbacks, geração de PWM, etc.).
- Em Desenvolvimento...

## Boas práticas
- Evite acessar canais analógicos fora de 0..5 (a função `read_analog` retorna 0 nesses casos).
- Proteja seções críticas se utilizar variáveis compartilhadas entre ISRs e o código principal (use `volatile` e/ou desabilite interrupções quando necessário).

## Autores
- [@Pedraum1](https://github.com/Pedraum1) - Criador e Idealizador