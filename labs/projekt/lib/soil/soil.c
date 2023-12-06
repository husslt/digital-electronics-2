#include <avr/io.h>         // AVR device-specific IO definitions
#include <soil.h>
#include <gpio.h>

uint16_t getSoilHumidityPercentage() {
    volatile uint16_t ADCvalue;          // value returned by ADC
    float percent;              // percentage calculation handling
    uint8_t pct;                // return value
    uint16_t max = 512;         // 0-1024 ... 0-5V, set maximum level of ADC, which will equal to 100 %, e.g. max voltage is 2,5V, then value is 2,5/5 * 1024 = 
    uint16_t min = 246;         // 0-1024, set minimum voltage corresponding to 0 %
    ADMUX &= ~(1<<REFS0); ADMUX |= (1<<REFS0); // Set reference Vref to AVCC, REFS1 = 0, REFS0 = 1
    ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0)); // Set ADC input source to ADC0, MUX3:0 = 0000
    ADMUX &= ~(1<<ADLAR);       // Set result register to be right adjusted, ADLAR = 0
    ADCSRA |= (1<<ADEN);        // Enable ADC conversion, ADEN = 1
    //ADCSRA |= (1<<ADIE);      // Enable ADC interrupt, ADIE = 1
    ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); // Set frequency division factor to 128, Arduino Uno clock = 16 MHz, ADC clock has to be between 50 and 200 kHz
    ADCSRA |= (1<<ADSC);        // Start ADC conversion
    while ((ADCSRA&(1<<ADIF)) == 0) {;}; // wait for conversion to finish (polling method)
    ADCvalue = ADC;             // ADC finished, put result variable into ADCvalue
    ADCSRA &= ~(1<<ADIF);       // clear ADIF after conversion (ADIF is not cleared automatically unless we are using interrupt method)
    percent = (float)(ADCvalue-min)/(max-min)*100; // calculate percentage, normalized to the interval defined by max and min variables
    pct = (int)percent;         // convert float back to int (with a small flooring error)
    return pct;
}

uint16_t getTankLevelPercentage() {
    uint16_t ADCvalue;          // value returned by ADC
    float percent;              // percentage calculation handling
    uint8_t pct;                // return value
    uint16_t max = 512;         // 0-1024 ... 0-5V, set maximum level of ADC, which will equal to 100 %, e.g. max voltage is 2,5V, then value is 2,5/5 * 1024 = 
    uint16_t min = 246;         // 0-1024, set minimum voltage corresponding to 0 %
    ADMUX &= ~(1<<REFS0); ADMUX |= (1<<REFS0); // Set reference Vref to AVCC, REFS1 = 0, REFS0 = 1
    ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1)); ADMUX |= (1<<MUX0); // Set ADC input source to ADC1, MUX3:0 = 0001
    ADMUX &= ~(1<<ADLAR);       // Set result register to be right adjusted, ADLAR = 0
    ADCSRA |= (1<<ADEN);        // Enable ADC conversion, ADEN = 1
    //ADCSRA |= (1<<ADIE);      // Enable ADC interrupt, ADIE = 1
    ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); // Set frequency division factor to 128, Arduino Uno clock = 16 MHz, ADC clock has to be between 50 and 200 kHz
    ADCSRA |= (1<<ADSC);        // Start ADC conversion
    while ((ADCSRA&(1<<ADIF)) == 0){;}; // wait for conversion to finish (polling method)
    ADCvalue = ADC;             // ADC finished, put result variable into ADCvalue
    ADCSRA &= ~(1<<ADIF);       // clear ADIF after conversion (ADIF is not cleared automatically unless we are using interrupt method)
    percent = (float)(ADCvalue-min)/(max-min)*100; // calculate percentage, normalized to the interval defined by max and min variables
    pct = (int)percent;         // convert float back to int (with a small flooring error)
    return pct;
}

void pumpTurnOff() {
    GPIO_write_low(&DDRB, PB0);
}

void pumpTurnOn() {
    GPIO_write_high(&DDRB, PB0);
}
