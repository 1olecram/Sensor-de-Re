#ifndef F_CPU
#define F_CPU 16000000UL // Define a frequência de 16MHz do Uno para os delays funcionarem
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> // Necessário para a função itoa()
#include <avr/interrupt.h>
#include "lcd_lib.h"
#include "hc_lib.h"
#include "buzzer_lib.h"

volatile uint16_t distancia_medida = 0;

ISR(INT0_vect)
{
    if (PIND & (1 << PD2))
    {
        // Echo subiu: Pulso começou (ligar cronômetro)
        TCNT2 = 0; // Zera o contador
        // Liga Timer2 com Prescaler de 1024 (CS22, CS21, CS20 = 1)
        TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
    }
    else
    {
        // Echo desceu: Pulso terminou (parar cronômetro)
        TCCR2B = 0; // Desliga Timer2
        uint8_t ticks = TCNT2;

        // Calcula a distância baseada no tempo do Timer2
        distancia_medida = (ticks * 64) / 58;
    }
}

// Essa interrupção impede que o sistema congele calculando lixo.
ISR(TIMER2_OVF_vect)
{
    TCCR2B = 0;             // Desliga Timer2
    distancia_medida = 255; // Força uma distância máxima (seguro)
}

int main(void)
{
    lcd_init();
    hc_sr04_int0_init();
    buzzer_init();

    sei(); // Habilita interrupções globais

    char buffer_dist[6];
    const uint16_t DISTANCIA_MAX_ALERTA = 35;
    const uint16_t DISTANCIA_CRITICA = 5;

    while (1)
    {
        // Envia o pulso de Trigger
        PORTB |= (1 << PB1);
        _delay_us(10);
        PORTB &= ~(1 << PB1);

        // Aguarda um curto período para a interrupção física fazer a medição no fundo
        _delay_ms(50);

        // Copia a variável global para o laço local
        uint16_t dist = distancia_medida;

        // Atualiza Display
        lcd_setCursor(0, 0);
        lcd_print("Dist: ");
        itoa(dist, buffer_dist, 10);
        lcd_print(buffer_dist);
        lcd_print(" cm     ");

        lcd_setCursor(0, 1);

        // Lógica do Buzzer
        if (dist > 0 && dist <= DISTANCIA_MAX_ALERTA)
        {

            if (dist <= DISTANCIA_CRITICA)
            {
                lcd_print("ALERTA: CRITICO ");
                buzzer_on();
                _delay_ms(100);
            }
            else
            {
                lcd_print("ALERTA: PERTO   ");

                buzzer_on();
                _delay_ms(60);
                buzzer_off();

                uint16_t tempo_silencio = dist * 12;
                for (uint16_t i = 0; i < tempo_silencio; i++)
                {
                    _delay_ms(1);
                }
            }
        }
        else
        {
            lcd_print("Status: Seguro  ");
            buzzer_off();
            _delay_ms(100);
        }
    }

    return 0;
}