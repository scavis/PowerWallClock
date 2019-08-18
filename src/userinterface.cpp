/**************************************************************************/
/*!
    @file     userinterface.cpp
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ESP32 arduino libraries
#include <Arduino.h>

// include Fri3d badge libraries
#include <Fri3dLegs.h>
#include <Fri3dMatrix.h>
#include <Fri3dBuzzer.h>
#include <Fri3dButtons.h>

// include custom Accelerometer lib
#include <Accelerometer.h>

// Project libraries
#include "userinterface.h"
#include "wallclock.h"

#include "global.h"
#include "custom.h"

// ==========================================>>
#ifdef __cplusplus
extern "C" {
#endif
uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();
// =========================================>>



UserInterface::UserInterface(WallClock* pcs) {
    // UserInterface constructor
    pwrcs = pcs;
}

void UserInterface::begin() {
    // UserInterface initializer
    accel.begin();
}
  
void UserInterface::update(unsigned int state) {
    // Run task by sequencer state.
    
    // Task: Update matrix screen.
    if ((state & UPD_MATRIX_EV ) == UPD_MATRIX_EV) {
        set_matrix();
    }
    
    // Task: Update wallclock state.
    if ((state & UPD_WALLCLK_EV ) == UPD_WALLCLK_EV) {
        set_wallclock();
    }
    
    // Task: Update badge sound => sound alarm if needed.
    if ((state & UPD_SOUND_EV ) == UPD_SOUND_EV) {
        set_sound();
    }
    
    // Task: Update wallclock state by user ( buttons ).
    if ((state & UPD_USRMODE_EV ) == UPD_USRMODE_EV) {
        set_usermode_button();
    }

    // Task: Update wallclock state by user ( accel ).
    if ((state & UPD_GACCEL_EV ) == UPD_GACCEL_EV) {
        set_usermode_nudge();
    }

    // Task: Update wallclock temperature
    if ((state & UPD_SENDTMP_EV ) == UPD_SENDTMP_EV) {
        set_temperature();
    }
}

void UserInterface::set_temperature(void) {
    //*
    uint8_t tempval = temprature_sens_read();
    
    pwrcs->set_temperature((uint)tempval);

}

void UserInterface::set_wallclock(void) {
    // Update global wallclock state (master => slave(s) ).
    pwrcs->update_wallclock();
}

void UserInterface::set_usermode_nudge(void){
    // Promote new clockstate by accel state.
    bool is_nudged = accel.is_triggered();

    if ( is_nudged ) {
        pwrcs->update_clockstate(STATE_TRIGGER);
    }
}

void UserInterface::set_usermode_button(void) {
    // Promote new clockstate by button state.
    bool buttn1 = buttons.getButton( 1 );
    bool buttn0 = buttons.getButton( 0 );

    // Check trigger button
    if ( buttn1 == 0b1) {
        pwrcs->update_clockstate(STATE_TRIGGER);
    } else

    // check disable button
    if ( buttn0 == 0b1) {
        pwrcs->update_clockstate(STATE_DISABLE);
    }
}

void UserInterface::set_sound(void) {
    // Update badge sound (fri3d badge)
    bool set_sound = pwrcs->is_alarm_active();
    
    if ( set_sound ) {
        buzzer.setVolume( ALM_VOLUME );
        buzzer.setFrequency( 500 );
    } else {
        buzzer.setVolume( 0 );
        buzzer.setFrequency( 0 );
    }
}

void UserInterface::set_matrix(void) {
    // Update led matrix (fri3d badge)
    String formattedChar = "0123456789";

    u_int wallclock = pwrcs->get_wallclock();
    bool usermode = pwrcs->is_clock_active();

    static u_int prev_wallclock = 0;
    u_int val;

    // visualize updated timer ( no update when equal )
    if ( prev_wallclock != wallclock) {
        matrix.clear();

        if ( (usermode == ENABLED) && (wallclock < WALLCLOCK_INFF) ) {

            val = ( ( wallclock / 1 ) % 10 );
            matrix.drawCharacter(11, formattedChar[val]);

            val = ( ( wallclock / 10 ) % 10 );
            matrix.drawCharacter(7, formattedChar[val]);

            val = ( ( wallclock / 100 ) % 10 );
            matrix.drawCharacter(4, formattedChar[val]);

            val = ( ( wallclock / 1000 ) % 10 );
            matrix.drawCharacter(0, formattedChar[val]);

        } else {
            matrix.drawCharacter(0, '#');
            matrix.drawCharacter(4, '#');
            matrix.drawCharacter(7, '#');
            matrix.drawCharacter(11,'#');
        }
    }
    prev_wallclock = wallclock;
}
