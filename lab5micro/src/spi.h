// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID:
// Date: 14/11/2024
// Assignment: Lab 4
//

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

void initspi();
void spi_send(uint8_t data);
void send_max7129(uint8_t reg, uint8_t data);
void init_max7129();
void max7219_display_pattern();
void max7219_display_sad_pattern();

#endif
