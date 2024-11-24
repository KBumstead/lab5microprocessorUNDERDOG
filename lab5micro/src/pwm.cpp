// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID: 
// Date: 14/11/2024
// Assignment: Lab 4
//


#include <avr/io.h>
#include "PWM.h"
#include <Arduino.h>


void initPWM()
{
    // Set up PWM on Pin 5 for speed control via EN1
    TCCR3A = (1 << COM3A1) | (1 << WGM31);              // Fast PWM, non-inverting
    TCCR3B = (1 << WGM32) | (1 << WGM33) | (1 << CS31); // Prescaler 8, using ICR3 as TOP
    DDRE |= (1 << PE3);                                 // PE3 (OC3A) as output for Buzzer
}

void setBuzzerFrequency(uint16_t frequency)
{
    //Setting the frequency using formula f = clock frequency / (prescaler * (ICR + 1))
    //Rearrange for ICR = (Clock frequency / (Prescaler * f)) - 1
    uint16_t topValue = (16000000 / (8 * frequency)) - 1;  // 16 MHz clock, prescaler 8

    ICR3 = topValue;
    OCR3A = topValue / 2;

    Serial.print("Frequency: ");
    Serial.println(frequency);
    Serial.print("ICR3 Value: ");
    Serial.println(ICR3);  // Print the calculated ICR3 value
}

void buzzerChirp()
{
    //How to produce the chirping sound effect

    // Sweep frequency from 1000 Hz to 5000 Hz and back down
    for (uint16_t freq = 1000; freq <= 5000; freq += 100) 
    { 
        setBuzzerFrequency(freq); // Set the buzzer frequency
        _delay_ms(10);
    }

    // Sweep frequency from 5000 Hz to 1000 Hz
    for (uint16_t freq = 5000; freq >= 1000; freq -= 100) 
    {
        setBuzzerFrequency(freq); // Set the buzzer frequency
        _delay_ms(10);

    }
}