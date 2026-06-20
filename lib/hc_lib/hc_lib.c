#include "hc_lib.h"
void hc_sr04_int0_init() {
    // Configura Pino 9 (PB1) como Saída (Trigger)
    DDRB |= (1 << PB1);   
    PORTB &= ~(1 << PB1); 

    // Configura Pino 2 (PD2 / INT0) como Entrada (Echo)
    DDRD &= ~(1 << PD2);  
    PORTD &= ~(1 << PD2); // Sinal vem limpo do sensor

    // Configura a interrupção INT0 para qualquer mudança lógica
    // ISC01 = 0 e ISC00 = 1
    EICRA &= ~(1 << ISC01);
    EICRA |= (1 << ISC00);

    // Habilita o INT0
    EIMSK |= (1 << INT0);
    
    // Habilita interrupção de overflow do Timer2 para evitar travamentos
    TIMSK2 |= (1 << TOIE2); 
}