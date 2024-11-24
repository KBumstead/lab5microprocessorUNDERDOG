// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID: 
// Date: 14/11/2024
// Assignment: Lab 4
//
#include "timer.h"


/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){              
     //initialize timer 0 and set it to ctc mode
    //initialize timer 0 and set it to ctc mode
    TCCR0A &= ~(1 << WGM00); //to clear the wgmo
    TCCR0A |= (1 << WGM10); //put 1 in wgm10 to set it to mode 2 which is ctc mode 2 010

    //to set prescaler to 64
    TCCR0B |= ((1 << CS01) | (1 << CS00));
    TCCR0B &= ~((1 << CS02) | (1 << WGM02));
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
    //because it is ctc mode, had to set our own count register which results in 249
    //by counting the OCR0A
    int prescaler = 64;
    OCR0A = ((0.001 * 16000000) / prescaler) - 1;
    
    for (unsigned int i = 0; i < delay; i++)
    {

        TCNT0 = 0;//to reset the counter in each loop to make sure it stays 0
        while (!(TIFR0 & (1<<OCF0A))) //loop to wait for the comparison if its both 1 then we go out from the loop
            ;
        TIFR0 |= (1 << OCF0A); //once the loop above finish tifr0 becomes 1 to show that the comparison has been reached.
    }
}