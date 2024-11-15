// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID: 
// Date: 14/11/2024
// Assignment: Lab 4
//


#include <avr/io.h>
#include "spi.h"
#include <Arduino.h>

void initspi(){
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
    //pin 51 is mosi 
    //pin 52 is clock SCk
    //pin 53 is SS

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

}

void spi_send(uint8_t data) {
    // Load data into the SPI data register
    SPDR = data;

    // Wait for transmission to complete (poll the SPIF flag)
    while (!(SPSR & (1 << SPIF)));

    // SPIF flag is set when transmission is complete
}

void send_max7129(uint8_t reg, uint8_t data){
    PORTB &= ~(1 << PB2); // Set CS low
    spi_send(reg);        // Send register address
    spi_send(data);       // Send data
    PORTB |= (1 << PB2);
}
void init_max7129() {
    send_max7129(0x09, 0x00); // No decode
    send_max7129(0x0A, 0x08); // Brightness
    send_max7129(0x0B, 0x07); // Scan all rows
    send_max7129(0x0C, 0x01); // Enable display
    send_max7129(0x0F, 0x00); // Disable test mode
}

void max7219_display_pattern() {
    uint8_t pattern[8] = {
        0b00000000, 
        0b00000000, 
        0b10000001, 
        0b10000001, 
        0b10000001, 
        0b10000001, 
        0b10000001, 
        0b11111111
    };

    for (uint8_t i = 0; i < 8; i++) {
        send_max7129(i + 1, pattern[i]);
    }
}