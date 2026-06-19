#include "hc_lib.h"
void hc_sr04_init() {
    DDRB |= (1 << PB1);   // Trigger (Pino 9) como SAÍDA
    DDRB &= ~(1 << PB0);  // Echo (Pino 8) como ENTRADA
    PORTB &= ~(1 << PB1); // Garante Trigger em LOW Inicial
    
    // Inicialização do Timer1 (Modo Normal)
    // TCCR1A = 0;
    // TCCR1B = 0; 
}

uint16_t get_distance() {
    // 1. Envia um pulso de 10 microssegundos no Trigger
    PORTB |= (1 << PB1);
    _delay_us(10);
    PORTB &= ~(1 << PB1);

    // 2. Aguarda o pino Echo (PB0) subir para HIGH (com timeout de segurança)
    uint16_t timeout = 0;
    while (!(PINB & (1 << PB0))) {
        timeout++;
        if (timeout > 60000) return 0; 
    }

    // // 3. Inicia a contagem de tempo usando o Timer1
    // TCNT1 = 0; // Reseta o contador
    // TCCR1B |= (1 << CS11); // Liga o Timer1 com Prescaler de 8 (1 tick = 0.5us)

    // // 4. Aguarda o pino Echo (PB0) descer para LOW
    // while (PINB & (1 << PB0)) {
    //     if (TCNT1 > 40000) break; // Limite de leitura segura (~3.5 metros)
    // }
    // TCCR1B &= ~(1 << CS11); // Desliga o Timer1

    // // 5. Converte os ciclos do temporizador em centímetros
    // uint16_t ticks = TCNT1;
    // return (ticks / 116);

    // 3. Mede a largura do pulso em microsegundos via software
    uint16_t us = 0;
    while (PINB & (1 << PB0)) {
        _delay_us(1);
        us++;
        if (us > 25000) break; // Timeout de segurança (~4 metros)
    }

    // 4. Converte microsegundos em centímetros
    return (us / 58);
}