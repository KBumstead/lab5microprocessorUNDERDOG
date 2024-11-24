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
#include "timer.h"

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
// define state button
typedef enum
{
  RELEASED,
  DB_RELEASED,
  PRESSED,
  DB_PRESSED,
} machineState;

// define state
typedef enum
{
  HAPPY_FACE,
  HAPPY_TO_SAD,
  SAD_FACE,
  SAD_TO_HAPPY
} emotion;

volatile machineState MachineState = RELEASED;
volatile emotion Emotion = HAPPY_FACE;



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

  startI2C_Trans(SLA);
  write(PWR_MGMT_1); // address on SLA for Power Management
  write(wakeup);   // send data to Wake up from sleep mode
  stopI2C_Trans();

  startI2C_Trans(SLA);
  write(ACCEL_CONFIG);
  write(AFS_SEL_2);
  stopI2C_Trans();


  while (1)
  {
      if(MachineState == DB_PRESSED){
      delayMs(50); //delay
      MachineState = PRESSED; //moving the state to pressed

    }
    else if(MachineState == DB_RELEASED){
      delayMs(50); //delay 
      MachineState = RELEASED; //moving the state to released
    }
    //the state of the machine when it is pressed it will activate the sevensegment and stop the motor from moving
    else if(MachineState == PRESSED){
      stopBuzzerSound();
    }
    //this activate when the state is in released basically reading the adc value and put into the changedutycycle for the motor to move in a speed and direction
    else if(MachineState == RELEASED){
      buzzerChirp();
    }

    max7219_display_pattern();

    Serial.print(1);
    //x value 
    read_from(SLA, ACCEL_XOUT_H);
    x_val = read_data(); //read upper value

    read_from(SLA, ACCEL_XOUT_L);
    x_val = (x_val << 8) | read_data(); //shift bit to the right and put in in the lower value

    x_accel = x_val/16384.0;
    //y value
    read_from(SLA, ACCEL_YOUT_H);
    y_val = read_data();
    read_from(SLA, ACCEL_YOUT_L);
    y_val = (y_val << 8) | read_data(); 

       y_accel = y_val/16384.0;
    //z value
    read_from(SLA, ACCEL_ZOUT_H);
    z_val = read_data();
    read_from(SLA, ACCEL_ZOUT_L);
    z_val = (z_val<<8)|read_data();
    z_accel = z_val/16384.0;
    Serial.println("testtttt");
    Serial.print("X-Axis =  ");
    Serial.print(x_accel);
    Serial.print(",  Y-Axis = ");
    Serial.print(y_accel);
    Serial.print(",  Z-Axis = ");
    Serial.println(z_accel);
    Serial.println(pitch);
    stopI2C_Trans();

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