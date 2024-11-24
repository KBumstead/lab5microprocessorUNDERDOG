// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID: 
// Date: 14/11/2024
// Assignment: Lab 4
//

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void initPWM();
void buzzerChirp();
void setBuzzerFrequency(uint16_t frequency);
void stopBuzzerSound();

#endif // PWM_H