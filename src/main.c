#ifndef F_CPU
#define F_CPU 16000000UL // Define a frequência de 16MHz do Uno para os delays funcionarem
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> // Necessário para a função itoa()
#include "lcd_lib.h"
#include "hc_lib.h"

int main(void) {
    lcd_init();
    hc_sr04_init();

    uint16_t distancia = 0;
    char buffer_dist[6];
    const uint16_t DISTANCIA_LIMITE = 15; // Limite de aproximação em centímetros

    while (1) {
        distancia = get_distance();

        // Linha 1: Sempre mostra a distância atualizada
        lcd_setCursor(0, 0);
        lcd_print("Dist: ");
        itoa(distancia, buffer_dist, 10);
        lcd_print(buffer_dist);
        lcd_print(" cm     "); // Espaços limpam dígitos antigos residuais

        // Linha 2: Altera a mensagem baseado no limiar de distância
        lcd_setCursor(0, 1);
        if (distancia <= DISTANCIA_LIMITE && distancia > 0) {
            lcd_print("ALERTA: PERTO!  ");
        } else {
            lcd_print("Status: Seguro  ");
        }

        _delay_ms(200); // Intervalo recomendado entre leituras do HC-SR04
    }

    return 0;
}