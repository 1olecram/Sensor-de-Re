#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <util/delay.h>
#include "lcd_lib.h"
void lcd_pulse_en() {
    PORTB |= (1 << PB3);  // EN = HIGH
    _delay_us(1);         // Aguarda 1 microssegundo
    PORTB &= ~(1 << PB3); // EN = LOW
    _delay_us(100);       // Dá tempo para o LCD processar
}

// Função para enviar 1 byte completo (dividido em 2 pedaços de 4 bits)
void lcd_send(uint8_t value, uint8_t is_data) {
    if (is_data) {
        PORTB |= (1 << PB4);  // RS = HIGH -> caractere/dado
    } else {
        PORTB &= ~(1 << PB4); // RS = LOW -> comando de controle
    }

    // Envia os 4 bits mais significativos
    PORTD &= ~((1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3)); // Zera os pinos de dados
    
    // Mapeamento do bit N 
    if (value & (1 << 4)) PORTD |= (1 << PD6);
    if (value & (1 << 5)) PORTD |= (1 << PD5);
    if (value & (1 << 6)) PORTD |= (1 << PD4);
    if (value & (1 << 7)) PORTD |= (1 << PD3);
    lcd_pulse_en();

    // Envia os 4 bits menos significativos
    PORTD &= ~((1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3)); // Zera os pinos de dados
    
    // Mapeamento do bit N
    if (value & (1 << 0)) PORTD |= (1 << PD6);
    if (value & (1 << 1)) PORTD |= (1 << PD5);
    if (value & (1 << 2)) PORTD |= (1 << PD4);
    if (value & (1 << 3)) PORTD |= (1 << PD3);
    lcd_pulse_en();
}

// Inicializa o LCD no modo 4 bits
void lcd_init() {
    // Configura os pinos RS(PB4), EN(PB3) e D4 a D7 (PD6 a PD3) como SAÍDA (1)
    DDRB |= (1 << PB4) | (1 << PB3);
    DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3);

    _delay_ms(50); // Aguarda o LCD ligar fisicamente

    // Sequência de inicialização obrigatória do datasheet do HD44780
    PORTB &= ~(1 << PB4); // RS = 0
    PORTD &= ~((1 << PD6) | (1 << PD5) | (1 << PD4) | (1 << PD3));
    
    // Envia o comando 0x03 três vezes e depois 0x02 (Ativa modo 4 bits)
    PORTD |= (1 << PD6) | (1 << PD5); // Seta D4 e D5
    lcd_pulse_en(); _delay_ms(5);
    lcd_pulse_en(); _delay_us(150);
    lcd_pulse_en();
    
    PORTD &= ~(1 << PD6); // Seta apenas D5 para enviar 0x02
    lcd_pulse_en();

    // Agora que está em 4 bits, usamos nossa função lcd_send
    lcd_send(0x28, 0); // Modo 4 bits, 2 linhas, matriz 5x8
    lcd_send(0x0C, 0); // Liga o display, desliga o cursor
    lcd_send(0x06, 0); // Modo de entrada (incrementa o cursor para a direita)
    lcd_send(0x01, 0); // Limpa a tela
    _delay_ms(2);      // Limpar a tela exige um delay maior
}

// Função para imprimir strings (textos)
void lcd_print(const char* str) {
    while (*str) {
        lcd_send(*str++, 1);
    }
}

// Função para posicionar o cursor (Coluna X, Linha Y)
void lcd_setCursor(uint8_t col, uint8_t row) {
    uint8_t row_offsets[] = { 0x00, 0x40 }; // Endereços da linha 0 e linha 1 na memória do LCD
    lcd_send(0x80 | (col + row_offsets[row]), 0);
}