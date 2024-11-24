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
    PRR0 &= ~(1 << PRTWI); //by putting it to 0 we enable power to i2c

    TWSR &= ~((1 << TWPS0) | (1 << TWPS1)); //prescaler to 1

    TWBR = 0xC6;  //set twbr to get SCL 10k

    TWCR |= (1 << TWINT) | (1 << TWEN); //enter MT mode tp enable 2 
}   


void startI2C_Trans(char unsigned SLA){
    TWCR |= (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); //to initiate the start condition
    
    //below line wait for twint flag set. this line inidicates the start condition has been transmitted
    while (!(TWCR & (1<<TWINT))); 

    TWDR |= (SLA << 1); //TWDR is set to the slave address to enter MT mode
    TWCR |= (1 << TWINT) | (1 << TWEN); // clear twint flag, enable TWI

    while (!(TWCR & (1 << TWINT))); //to wait twint high so that the data can be transmitted
}

void stopI2C_Trans(){
    TWCR |= (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); //stop condition to write to the twcr

}

void write(unsigned char data){
    TWDR = data; //write data to the TWDR
    TWCR = (1 << TWINT) | (1 << TWEN); // clear twint flag and continue 

    while (!(TWCR & (1 << TWINT)));//wait twint high so that data can be transmited
}

void read_from(unsigned char SLA, unsigned char MEMADDRESS){
    startI2C_Trans(SLA); //start i2c trnasfer to slave
    write(MEMADDRESS); //write to memory address

    TWCR |= (1 << TWINT) | (1<<TWSTA) | (1 << TWEN); //clear twint and start read condition
    while (!(TWCR & (1 << TWINT))); //waiting for completion
    
    TWDR |= (SLA << 1) | 0x01;  //set data register to SLA and read bit

    TWCR |= (1 << TWINT) | (1 << TWEA) | (1 << TWEN) ; //trigger action and master acknowledge(TWEA)
    while (!(TWCR & (1 << TWINT)));//wait twint high so that data can be transmited

    TWCR |= (1 << TWINT) | (1 << TWEN); // trigger action
    while (!(TWCR & (1 << TWINT)));//wait twint high so that data can be transmited

    TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); //trigger action and stop condition (TWSTO)
}

unsigned char read_data(){

    return TWDR; //return TWDR
    
}