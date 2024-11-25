// #######################################################################//
//  Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
//  Net ID: kenjiegozali, rohitvasandani
//  Date: 25/11/2024
//  Assignment: Lab 5
//----------------------------------------------------------------------//

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void initPWM();
void buzzerChirp();
void setBuzzerFrequency(uint16_t frequency);
void stopBuzzerSound();
void turnOnBuzzerPin();
void turnOffBuzzerPin();

#endif // PWM_H