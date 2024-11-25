// #######################################################################//
//  Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
//  Net ID: kenjiegozali, rohitvasandani
//  Date: 25/11/2024
//  Assignment: Lab 5
//----------------------------------------------------------------------//

#ifndef I2C.H
#define I2C .H

#include <avr/io.h>

void initI2C();
void startI2C_Trans(char unsigned SLA);
void stopI2C_Trans();
void write(unsigned char data);
void read_from(unsigned char SLA, unsigned char MEMADDRESS);
unsigned char read_data();

#endif
