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
#include <soil.h>  
#include <stdio.h>
#include <gpio.h>

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/

//uint16_t ADCvalue; // variable to be used by interrupt routine


int main(void)
{
    oled_init(OLED_DISP_ON);
    oled_clrscr();
    oled_charMode(NORMALSIZE);
    oled_gotoxy(0,0);
    oled_puts("SOIL: ");
    oled_gotoxy(0,1);
    oled_puts("ADCTANK: ");
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
    uint8_t soil = getSoilHumidityPercentage();
    uint8_t tank = getTankLevelPercentage();
    char string[3];
    itoa(soil, string, 10);
    oled_gotoxy(6,0);
    oled_puts(string);
    itoa(tank, string, 10);
    oled_gotoxy(9,1);
    oled_puts(string);
    oled_display();
    if (tank < 50) {
        pumpTurnOn();
    }
    if (tank > 50) {
        pumpTurnOff();
    }
}


