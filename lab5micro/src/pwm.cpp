// #######################################################################//
//  Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
//  Net ID: kenjiegozali, rohitvasandani
//  Date: 25/11/2024
//  Assignment: Lab 5
//----------------------------------------------------------------------//

#include <avr/io.h>
#include "PWM.h"
#include <Arduino.h>
#include "timer.h"

void initPWM()
{
    // Set up PWM on Pin 5 for speed control via EN1
    TCCR3A = (1 << COM3A1) | (1 << WGM31);              // Fast PWM, non-inverting
    TCCR3B = (1 << WGM32) | (1 << WGM33) | (1 << CS31); // Prescaler 8, using ICR3 as TOP
    // PE3 (OCR3A) as output for Buzzer
}

void setBuzzerFrequency(uint16_t frequency)
{
    // Setting the frequency using formula f = clock frequency / (prescaler * (ICR + 1))
    // Rearrange for ICR = (Clock frequency / (Prescaler * f)) - 1
    uint16_t topValue = (16000000 / (8 * frequency)) - 1; // 16 MHz clock, prescaler 8

    ICR3 = topValue;      // setting the (frequency) top value
    OCR3A = topValue / 2; // basically setting the (you could say volume) the timer it need to count to
}

void buzzerChirp()
{

    //turns the pin on when wants to chirp to make sure buzzer doesnt make a sound during idle
    DDRE |= (1 << PE3);

    // How to produce the chirping sound effect
    
    // Sweep frequency from 1000 Hz to 5000 Hz and back down
    for (uint16_t freq = 1000; freq <= 5000; freq += 500)
    {
        setBuzzerFrequency(freq); // Set the buzzer frequency
        delayMs(10);              // delay
    }

    // Sweep frequency from 5000 Hz to 1000 Hz
    for (uint16_t freq = 5000; freq >= 1000; freq -= 500)
    {
        setBuzzerFrequency(freq); // Set the buzzer frequency
        delayMs(10);              // delay
    }
}
void stopBuzzerSound()
{
    OCR3A = 0; // make ocr3a 0 to stop the counting
}