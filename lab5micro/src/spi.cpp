// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID:
// Date: 14/11/2024
// Assignment: Lab 4
//

#include <avr/io.h>
#include "spi.h"
#include <Arduino.h>

void initspi()
{
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
    // mosi == DDB2
    // SCK == DDB1 is clock SCk
    // SS DDB0
    // PORTB |= (1 << PB0);

    // enable SPE, master and set SCK frequency to 16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_send(uint8_t data)
{
    // Load data into the SPI data register
    SPDR = data;

    // Wait for transmission to complete (poll the SPIF flag)
    while (!(SPSR & (1 << SPIF)))
        ;
    // SPIF flag is set when transmission is complete
}

void send_max7129(uint8_t address, uint8_t data)
{
    PORTB &= ~(1 << DDB1); // Set CS low
    spi_send(address);     // Send register address
    spi_send(data);        // Send data
    PORTB |= (1 << DDB1);
}
void init_max7129()
{
    send_max7129(0x09, 0x00); // Decode mode
    send_max7129(0x0A, 0x0F); // Brightness maximum
    send_max7129(0x0B, 0x07); // Scan all rows
    send_max7129(0x0C, 0x01); // Enable display
    send_max7129(0x0F, 0x00); // Disable test mode
}

void max7219_display_pattern()
{

    send_max7129(0x01, 0b00000000);
    send_max7129(0x02, 0b01000010);
    send_max7129(0x03, 0b00000000);
    send_max7129(0x04, 0b00000000);
    send_max7129(0x05, 0b00111100);
    send_max7129(0x06, 0b01000010);
    send_max7129(0x07, 0b10000001);
    send_max7129(0x08, 0b00000000);
}

void max7219_display_sad_pattern()
{
    send_max7129(0x01, 0b00000000); // all LEDS in Row 1 are off
    send_max7129(0x02, 0b01000010); // row 2 LEDS
    send_max7129(0x03, 0b10100101); // row 3 LEDS
    send_max7129(0x04, 0b01000010); // row 4 LEDS
    send_max7129(0x05, 0b00000000); // row 5 LEDS
    send_max7129(0x06, 0b10000001); // row 6 LEDS
    send_max7129(0x07, 0b01111110); // row 7 LEDS
    send_max7129(0x08, 0b00000000);
}