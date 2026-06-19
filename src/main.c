#ifndef F_CPU
#define F_CPU 16000000UL // Define a frequência de 16MHz do Uno para os delays funcionarem
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> // Necessário para a função itoa()
#include "lcd_lib.h"
#include "hc_lib.h"
#include "buzzer_lib.h"

int main(void) {
    lcd_init();
    hc_sr04_init();
    buzzer_init();

    uint16_t distancia = 0;
    char buffer_dist[6];
    const uint16_t DISTANCIA_MAX_ALERTA = 35; // Começa a bipar a partir de 35 cm
    const uint16_t DISTANCIA_CRITICA = 5;     // Som contínuo abaixo de 5 cm

    while (1) {
        distancia = get_distance();

        // Atualização do LCD
        lcd_setCursor(0, 0);
        lcd_print("Dist: ");
        itoa(distancia, buffer_dist, 10);
        lcd_print(buffer_dist);
        lcd_print(" cm     ");

        lcd_setCursor(0, 1);

        // Lógica do Sensor de Estacionamento (Frequência dos bipes)
        if (distancia > 0 && distancia <= DISTANCIA_MAX_ALERTA) {
            
            if (distancia <= DISTANCIA_CRITICA) {
                // Muito perto: Emite som contínuo
                lcd_print("ALERTA: CRITICO ");
                buzzer_on();
                _delay_ms(100); 
            } 
            else {
                // Faixa de aproximação: bipa e altera o tempo de silêncio
                lcd_print("ALERTA: PERTO   ");
                
                buzzer_on();
                _delay_ms(60); // Duração física do bipe "bip"
                buzzer_off();

                // O tempo de silêncio é proporcional à distância.
                // Quanto menor a distância, menor o silêncio, aumentando a frequência dos bipes.
                uint16_t tempo_silencio = distancia * 12; 
                
                // Laço para gerar o delay dinâmico baseado na variável
                for (uint16_t i = 0; i < tempo_silencio; i++) {
                    _delay_ms(1);
                }
            }
        } 
        else {
            // Fora da zona de perigo
            lcd_print("Status: Seguro  ");
            buzzer_off();
            _delay_ms(100); // Janela padrão de espera
        }
    }

    return 0;
}