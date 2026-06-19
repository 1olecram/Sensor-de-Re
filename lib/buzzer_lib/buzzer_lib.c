#include "buzzer_lib.h"
void buzzer_init() {
    DDRB |= (1 << PB2); // Configura o Pino 10 (OC1B) como SAÍDA
    
    // Configura o Timer1 em modo CTC (Clear Timer on Compare Match) com OCR1A como TOP (Modo 4)
    // Esse modo limpa o contador toda vez que ele atinge o valor de OCR1A.
    TCCR1A = 0;
    TCCR1B = 0;
    
    // Calculo para frequência do som do bipe (ex: ~1.5 kHz para um som nítido):
    // OCR1A = (F_CPU / (2 * Prescaler * Freq_Desejada)) - 1
    // OCR1A = (16000000 / (2 * 8 * 1500)) - 1 = 665
    OCR1A = 665; 
}

void buzzer_on() {
    // COM1B0 = 1: Alterna o estado (Toggle) do pino OC1B (Pino 10) a cada correspondência.
    // Isso garante nativamente um Duty Cycle perfeito de 50% sem intervenção do processador.
    TCCR1A |= (1 << COM1B0);
    
    // WGM12 = 1 (Ativa modo CTC), CS11 = 1 (Prescaler de 8) -> Inicia o som
    TCCR1B |= (1 << WGM12) | (1 << CS11);
}

void buzzer_off() {
    TCCR1A &= ~(1 << COM1B0); // Desconecta o pino do Timer
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // Para o clock do Timer
    PORTB &= ~(1 << PB2);     // Garante que o pino termine em nível BAIXO
}
