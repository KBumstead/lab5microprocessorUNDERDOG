// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID: 
// Date: 14/11/2024
// Assignment: Lab 4

#include <avr/io.h>
#include "i2c.h"
#include <Arduino.h>


void initI2C(){
    // DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);
    DDRD |= (1 << DDD0) | (1 << DDD1);
    //20 is SDA
    //21 is SCL
}   


void startI2C_Trans(char unsigned SLA){

}

void stopI2C_Trans(){

}

void write(unsigned char data){

}

void read_from(unsigned char SLA, unsigned char MEMADDRESS){

}

unsigned char read_data(){



}