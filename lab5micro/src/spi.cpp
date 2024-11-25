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
    PORTB |= (1 << PB0);

    // enable SPE, master and set SCK frequency to 16
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
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
    PORTB &= ~(1 << PORTB0); // Set CS low
    spi_send(address);       // Send register address
    spi_send(data);          // Send data
    PORTB |= (1 << PORTB0);  // set CS to high back
}
void init_max7129()
{
    send_max7129(0x0A, 0x08); // brightness control
    send_max7129(0x0B, 0x07); // scanning all rows and columns
    send_max7129(0x0C, 0x01); // set shutdown register to normal operation (0x01)
    send_max7129(0x0F, 0x00); // display test register - set to normal operation (0x00)
}

// fuctioj below to display a happy face
void max7219_display_pattern()
{

    send_max7129(0x01, 0b00000000); // first row
    send_max7129(0x02, 0b01000010); // second row
    send_max7129(0x03, 0b10100101); // third row
    send_max7129(0x04, 0b01000010); // fourth row
    send_max7129(0x05, 0b00000000); // fifth row
    send_max7129(0x06, 0b10000001); // sixth row
    send_max7129(0x07, 0b01111110); // seventh row
    send_max7129(0x08, 0b00000000); // eight row
}

void max7219_display_sad_pattern()
{
    send_max7129(0x01, 0b00000000); // first row
    send_max7129(0x02, 0b01000010); // second row
    send_max7129(0x03, 0b00000000); // third row
    send_max7129(0x04, 0b00000000); // fourth row
    send_max7129(0x05, 0b00111100); // fifth row
    send_max7129(0x06, 0b01000010); // sixth row
    send_max7129(0x07, 0b10000001); // sevent row
    send_max7129(0x08, 0b00000000); // eight row
}