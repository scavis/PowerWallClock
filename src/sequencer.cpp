/**************************************************************************/
/*!
    @file     sequencer.cpp
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ESP32 arduino libraries
#include <Arduino.h>

// Project libraries
#include "sequencer.h"

#include "global.h"
#include "custom.h"


Sequencer::Sequencer() {
    // Sequencer constructor
    begin();
}

void Sequencer::begin() {
    // Sequencer initializer
    curr_state = UPD_MATRIX_EV;
}

void Sequencer::update(uint* state) {
    // define next sequencer state
    static u_int wallclk_div = 0;

    uint sequencer_st = 0;

    delayMicroseconds(1000);

    // activate single state: next state to run

    if (curr_state == UPD_NOSTATE_EV) sequencer_st = sequencer_st | UPD_MATRIX_EV;

    if (curr_state == UPD_MATRIX_EV) sequencer_st = sequencer_st | UPD_PWRCLK_EV;
    if (curr_state == UPD_PWRCLK_EV) sequencer_st = sequencer_st | UPD_GACCEL_EV;
    if (curr_state == UPD_GACCEL_EV) sequencer_st = sequencer_st | UPD_SOUND_EV;
    if (curr_state == UPD_SOUND_EV)  sequencer_st = sequencer_st | UPD_SENDMSG_EV;
    if (curr_state == UPD_SENDMSG_EV) sequencer_st = sequencer_st | UPD_RECONN_EV;
    if (curr_state == UPD_RECONN_EV)  sequencer_st = sequencer_st | UPD_USRMODE_EV;
    if (curr_state == UPD_USRMODE_EV) sequencer_st = sequencer_st | UPD_WALLCLK_EV;
    if (curr_state == UPD_WALLCLK_EV) sequencer_st = sequencer_st | UPD_SENDTMP_EV;

    if (curr_state == UPD_SENDTMP_EV) sequencer_st = sequencer_st | UPD_MATRIX_EV;

    // todo: include detection whether a second has passed!
    if  ( ( sequencer_st & UPD_WALLCLK_EV) == UPD_WALLCLK_EV ) {
        if ( wallclk_div > 0 ) {
            wallclk_div -= 1;
            sequencer_st &= ~(UPD_WALLCLK_EV);
        } else {
            wallclk_div = 100;
        }
    }

    curr_state = sequencer_st;

    *state = sequencer_st;
}
