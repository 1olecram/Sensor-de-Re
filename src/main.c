#ifndef F_CPU
#define F_CPU 16000000UL // Define a frequência de 16MHz do Uno para os delays funcionarem
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> // Necessário para a função itoa()
#include "lcd_lib.h"

// Função para gerar o pulso no pino EN (Enable) que confirma o envio dos dados


int main(void) {
    // Substitui a função setup() do Arduino
    lcd_init();
    
    lcd_setCursor(0, 0);
    lcd_print("Ola, mundo!"); // "Olá" sem acento para evitar bugs na tabela de caracteres do LCD

    uint16_t segundos = 0;
    char buffer_texto[6];

    // Substitui a função loop() do Arduino
    while (1) {
        lcd_setCursor(0, 1);
        
        // Converte o número inteiro (segundos) em texto (string) base 10
        itoa(segundos, buffer_texto, 10);
        
        lcd_print(buffer_texto);
        lcd_print(" s  "); // Espaços extras apagam vestígios se os números mudarem de tamanho
        
        // Como não temos o millis() nativo, usamos um delay de 1 segundo para incrementar
        _delay_ms(1000); 
        segundos++;
    }

    return 0;
}