// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID: 
// Date: 14/11/2024
// Assignment: Lab 4
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//

#include <avr/io.h>

#include "pwm.h"
#include "spi.h"
#include "i2c.h"
#include "Arduino.h"

// defines
#include <avr/interrupt.h>

//define state
typedef enum
{
  RELEASED,
  DB_RELEASED,
  PRESSED,
  DB_PRESSED,
} machineState;


volatile machineState MachineState = RELEASED;
volatile bool countingDown = false;


int main()
{

  // init
  initspi();
  initI2C();
  init_max7129();

  Serial.begin(9600);  // Initialize serial communication
  sei(); //to enable interrupt 

  while (1)
  {
      max7219_display_pattern();
      max7219_display_sad_pattern();
      _delay_ms(1000);
  }

  
  return 0;
}

ISR(INT0_vect){
  // it checks the state of the button to if it is in a release state itll go to the debouncing pressed state
  if (MachineState == RELEASED){
     MachineState = DB_PRESSED;
    }
  else if(MachineState == PRESSED){
      //it checks the state of the MachineState to if it is in a pressed state itll go to the debouncing release state
      MachineState = DB_RELEASED;
    }

}