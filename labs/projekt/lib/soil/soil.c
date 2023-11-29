#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for number conversions

ISR(ADC_vect) {
    result = ADC;
}

uint16_t get_tank_level() {
    
    uint16_t result; // variable to be modified by interrupt
    ADMUX &= ~(1<<REFS0); ADMUX |= (1<<REFS0); // Set reference Vref to AVCC, REFS1 = 0, REFS0 = 1
    ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0)); // Set ADC input source to ADC0, MUX3:0 = 0000
    ADMUX &= ~(1<<ADLAR); // Set result register to be right adjusted, ADLAR = 0
    ADCSRA |= (1<<ADEN); // Enable ADC conversion, ADEN = 1
    ADCSRA |= (1<<ADIE); // Enable ADC interrupt, ADIE = 1
    ADCSRA &= ~((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); // Set frequency division factor to 128, Arduino Uno clock = 16 MHz, ADC clock has to be between 50 and 200 kHz
    ADCSRA |= (1<<ADSC); // Start ADC conversion
    
    return result;

}

uint16_t get_soil_humidity() {

}

void pump_turn_off() {

}

void pump_turn_on() {

}