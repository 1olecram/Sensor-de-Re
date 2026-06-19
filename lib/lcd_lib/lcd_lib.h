#include <avr/io.h>
void lcd_pulse_en();
void lcd_send(uint8_t value, uint8_t is_data);
void lcd_init ();
void lcd_print (const char* str);
void lcd_setCursor (uint8_t col, uint8_t row);