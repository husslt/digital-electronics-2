/***********************************************************************
 * 
 * Use Analog-to-digital conversion to read push buttons on LCD keypad
 * shield and display it on LCD screen.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <oled.h>
#include <stdlib.h>         // C library. Needed for number conversions


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    oled_init(OLED_DISP_ON);
    oled_clrscr();

    oled_gotoxy(0,0);
    oled_puts("BIN: ");
    oled_charMode(DOUBLESIZE);
    oled_gotoxy(0,2);
    oled_puts("DEC: ");
    oled_gotoxy(0,4);
    oled_puts("PCT: ");
    //oled_puts("OLED disp.");

    //oled_charMode(NORMALSIZE);

    // oled_gotoxy(x, y)
    //oled_gotoxy(0, 2);
    //oled_puts("128x64, SHH1106");

    // oled_drawLine(x1, y1, x2, y2, color)
    //oled_drawLine(0, 25, 120, 25, WHITE);

    //oled_gotoxy(0, 4);
    //oled_puts("BPC-DE2, Brno");

    // Copy buffer to display RAM
    oled_display();

    TIM1_OVF_1SEC
    TIM1_OVF_ENABLE
    

    sei();

    while (1) {
        ;
    }

    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
    ADMUX &= ~(1<<REFS0); ADMUX |= (1<<REFS0); // Set reference Vref to AVCC, REFS1 = 0, REFS0 = 1
    ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0)); // Set ADC input source to ADC0, MUX3:0 = 0000
    ADMUX &= ~(1<<ADLAR); // Set result register to be right adjusted, ADLAR = 0
    //ADMUX |= (1<<ADLAR); // Set result register ADCH:ADCL to be left adjusted, ADLAR = 1
    ADCSRA |= (1<<ADEN); // Enable ADC conversion, ADEN = 1
    ADCSRA |= (1<<ADIE); // Enable ADC interrupt, ADIE = 1
    ADCSRA &= ~((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)); // Set frequency division factor to 128, Arduino Uno clock = 16 MHz, ADC clock has to be between 50 and 200 kHz
    ADCSRA |= (1<<ADSC); // Start ADC conversion

}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/

ISR(ADC_vect)
{
    uint16_t value;
    uint8_t pct;
    uint16_t max = 1024;
    uint16_t min = 0;
    char string[4];
    char pct_string[3];  
    value = ADC;
    pct = (value-min)/(max-min) *100;
    oled_charMode(NORMALSIZE);
    itoa(value, string, 2);
    oled_gotoxy(4,0);
    oled_puts(string);
    oled_charMode(DOUBLESIZE);
    oled_gotoxy(8,2);
    itoa(value, string, 10);
    oled_puts(string);
    oled_gotoxy(8,4);
    itoa(pct, pct_string, 10);
    oled_puts(pct_string);
    oled_display();
}
