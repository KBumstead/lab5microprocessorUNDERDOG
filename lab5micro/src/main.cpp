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
#include <Arduino.h>

#include "pwm.h"
#include "spi.h"
#include "i2c.h"
#include "switch.h"
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

#define WHO_AM_I 0x75
// define state button
typedef enum
{
  RELEASED,
  DB_RELEASED,
  PRESSED,
  DB_PRESSED,
} machineState;

// define state of the spi
typedef enum
{
  HAPPY_FACE,
  HAPPY_TO_SAD,
  SAD_FACE,
  SAD_TO_HAPPY
} emotion;

volatile machineState MachineState = RELEASED;
volatile emotion Emotion = HAPPY_FACE;
volatile bool buzzerChirping = false;

int main()
{

  // init
  initTimer0();
  initspi();
  initI2C();
  init_max7129();
  initPWM();
  initSwitchPD2();

  Serial.begin(9600); // Initialize serial communication
  sei();              // to enable interrupt

  // declaring variable for counting the roll and pitch
  signed int x_val = 0;
  signed int y_val = 0;
  signed int z_val = 0;
  float x_accel = 0;
  float y_accel = 0;
  float z_accel = 0;
  float roll = 0;
  float pitch = 0;

  startI2C_Trans(SLA); // begin i2c communication with the slave
  write(PWR_MGMT_1);   // access the power management
  write(wakeup);       // write data to wake up the device from sleep mode
  stopI2C_Trans();     // end i2c communcation

  startI2C_Trans(SLA); // begin new i2c communication
  write(ACCEL_CONFIG); // access the sensor configuration register
  write(AFS_SEL_2);    // set accelrometer sensitiviy
  stopI2C_Trans();     // end i2c communication

  while (1)
  {

    // x value
    read_from(SLA, ACCEL_XOUT_H); // request the data from the slave of the high x axis
    x_val = read_data();          // read upper value

    read_from(SLA, ACCEL_XOUT_L);       // request the data for the low x axis
    x_val = (x_val << 8) | read_data(); // shift bit to put in in the lower value, combine the data from high also

    x_accel = x_val / 16384.0; // calculations to find the x-axis
    // y value
    read_from(SLA, ACCEL_YOUT_H);       // request the data from the slave for the high y axis
    y_val = read_data();                // read upper value
    read_from(SLA, ACCEL_YOUT_L);       // request data from slave for the low y axis
    y_val = (y_val << 8) | read_data(); // shift bit to put in in the lower value, combine the data from high also
    y_accel = y_val / 16384.0;          // calculations to find the y-axis

    // z value
    read_from(SLA, ACCEL_ZOUT_H);       // request data from the slave for the high z axis
    z_val = read_data();                // read upper value
    read_from(SLA, ACCEL_ZOUT_L);       // request data from the slave for the low z value
    z_val = (z_val << 8) | read_data(); // shift bit to put in in the lower value, combine the data from high
    z_accel = z_val / 16384.0;          // calculations to find the z-axis

    read_from(SLA, ACCEL_CONFIG); // request data from slave for the config registeration value

    roll = atan2(y_accel, sqrt(x_accel * x_accel + z_accel * z_accel)) * 180 / 3.14;         // count the roll angle which would be later for the state of emotion
    pitch = atan2(x_accel, sqrt(y_accel * y_accel + z_accel * z_accel)) * 180 / 3.14 - 0.12; // count pitch angle

    // serial print first
    // Serial.println("testtttt");
    // Serial.print("X-Axis =  ");
    // Serial.print(x_accel);
    // Serial.print(",  Y-Axis = ");
    // Serial.print(y_accel);
    // Serial.print(",  Z-Axis = ");
    // Serial.println(z_accel);
    // Serial.println(pitch);
    stopI2C_Trans();

    // below will be the state of emotion

    if (Emotion == HAPPY_FACE)
    {
      // when it is tilted to more than 45 degree it would change state from happy to sad
      if (roll > 45.0 || roll < -45.0 || pitch > 45.0 || pitch < -45.0)
      {
        delayMs(50); // delay and once again check if it really have tilted
        if (roll > 45.0 || roll < -45.0 || pitch > 45.0 || pitch < -45.0)
        {
          Emotion = HAPPY_TO_SAD; // change state
        }
      }
      // below if else function for to know if the buzzer still chirping or not
      if (buzzerChirping)
      {
        buzzerChirp(); // Continuously produce the chirping effect
      }
    }

    else if (Emotion == SAD_FACE) // same as above if the current state of emotion is sad face it would go through this if function
    {
      if (roll > -45.0 && roll < 45.0 && pitch >= -45.0 && pitch <= 45.0)
      // if it is not tilted or go back to once it was it will have delay and check it again before switchting state
      {
        delayMs(50);
        if (roll > -45.0 && roll < 45.0 && pitch >= -45.0 && pitch <= 45.0)
        {
          Emotion = SAD_TO_HAPPY;
        }
      }
      // to know if buzzer still chirping or not
      if (!buzzerChirping) // Start the chirping state
      {
        buzzerChirping = true;
      }
      // if true itll still chirp
      if (buzzerChirping)
      {
        buzzerChirp(); // Continuously produce the chirping effect
      }
    }
    else if (Emotion == SAD_TO_HAPPY)
    {
      max7219_display_pattern(); // show the happy pattern

      Emotion = HAPPY_FACE; // move state to happy face
    }
    else if (Emotion == HAPPY_TO_SAD)
    {
      max7219_display_sad_pattern(); // display sad pattern

      Emotion = SAD_FACE; // move state to sad face
    }

    // state of the button
    if (MachineState == RELEASED)
    {
    }
    else if (MachineState == PRESSED)
    {

      buzzerChirping = false;     // change buzzer to false
      DDRE &= ~(1 << PE3);        // make ddr register to 0
      delayMs(50);                // delay
      MachineState = DB_RELEASED; // move state
    }
    else if (MachineState == DB_PRESSED)
    {
      delayMs(50);            // delay
      MachineState = PRESSED; // movestate
    }
    else if (MachineState == DB_RELEASED)
    {

      delayMs(50);             // delay
      MachineState = RELEASED; // movestate
    }
  }
  return 0;
}

ISR(INT2_vect)
{
  // it checks the state of the button to if it is in a release state itll go to the debouncing pressed state
  if (MachineState == RELEASED)
  {
    MachineState = DB_PRESSED;
  }
  else if (MachineState == PRESSED)
  {
    // it checks the state of the MachineState to if it is in a pressed state itll go to the debouncing release state
    MachineState = DB_RELEASED;
  }
}
