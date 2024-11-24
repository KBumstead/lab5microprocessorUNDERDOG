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

#include <avr/interrupt.h>
// defines
#define SLA 0x68 
#define PWR_MGMT_1 0x6B
#define ACCEL_CONFIG 0x1C
#define wakeup 0x00
#define AFS_SEL_2 0x00

#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

#define WHO_AM_I 0x75
//define state button
// typedef enum
// {
//   RELEASED,
//   DB_RELEASED,
//   PRESSED,
//   DB_PRESSED,
// } machineState;

//define state
// typedef enum
// {
//   HAPPY_FACE,
//   HAPPY_TO_SAD,
//   SAD_FACE,
//   SAD_TO_HAPPY
// } emotion;

// volatile machineState MachineState = RELEASED;
// volatile emotion Emotion = HAPPY_FACE;



int main()
{

  // init
  initspi();
  initI2C();
  init_max7129();
  initPWM();

  Serial.begin(9600);  // Initialize serial communication
  sei(); //to enable interrupt 


  signed int x_val = 0;
  signed int y_val = 0;
  signed int z_val = 0;
  signed int moded = 2;
  float x_accel = 0;
  float y_accel = 0;
  float z_accel = 0;
  float roll = 0;
  float pitch = 0;


  while (1)
  {
    Serial.print(1);
    buzzerChirp();

    // //x value 
    // read_from(SLA, ACCEL_XOUT_H);
    // x_val = read_data(); //read upper value

    // read_from(SLA, ACCEL_XOUT_L);
    // x_val = (x_val << 8) | read_data(); //shift bit to the right and put in in the lower value

    // x_accel = x_val/16384.0;
    // //y value
    // read_from(SLA, ACCEL_YOUT_H);
    // y_val = read_data();

    // read_from(SLA, ACCEL_YOUT_L);
    // y_val = (y_val << 8) | read_data(); 

    
    // z value
    // read_from(sla, ACCEL_ZOUT_H);

  }
  return 0;
}

// ISR(INT0_vect){
//   // it checks the state of the button to if it is in a release state itll go to the debouncing pressed state
//   if (MachineState == RELEASED){
//      MachineState = DB_PRESSED;
//     }
//   else if(MachineState == PRESSED){
//       //it checks the state of the MachineState to if it is in a pressed state itll go to the debouncing release state
//       MachineState = DB_RELEASED;
//     }

// }