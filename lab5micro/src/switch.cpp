// Author: Kenjie Aaron Gozali and Rohit Jathniel Vasandani
// Net ID: 
// Date: 14/11/2024
// Assignment: Lab 4
//


// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on D0    and sets it into input mode
 */
void initSwitchPDO(){
    //pull up resistor on D0 set it into input mode
    PORTD |= (1 << PORTD0);
    DDRD &= ~(1 << DDD0);


    //to trigger the external interrupt switch by clearing put isc01 to 1 and ics00 to 0 so that the switch can detect an input/interrupt when the 
    //switch is going from high to low 
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);

    //this below enables the int0 for the external intterupt int0 
    EIMSK |= (1 << INT0);
}
