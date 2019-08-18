/**************************************************************************/
/*!
    @file     wallclock.cpp
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/


// ESP32 arduino libraries
#include <Arduino.h>

// Project libraries
#include "wallclock.h"

#include "global.h"
#include "custom.h"


WallClock::WallClock() {
    // WallClock constructor
    begin();
}

void WallClock::begin(void) {
    // WallClock initializer
    timestate = WALLCLOCK_SHORT;
    cur_state = STATE_MKEXIT;
    new_state = STATE_MKEXIT;
    usr_event = EVENT_INIT;

    alarm_enabled = true;
}

bool WallClock::is_power_active(void) {
    // is wallclock power level high
    return ( timestate > 0);
}

bool WallClock::is_clock_active(void) {
    // is wallclock timer running
    return (cur_state == STATE_NORMAL);
}

bool WallClock::is_alarm_active(void) {
    // is wallclock alarm sound active
    if ( timestate < ALM_TIME && timestate > 0x0 ) {
        return alarm_enabled;
    }
    return false;
}

uint WallClock::get_wallclock(void) {
    // get wallclock timer
    return timestate;
}

uint WallClock::get_temperature(void){
    // get temperature value
    return temperature;
}

void WallClock::set_wallclock(uint time){
    // set wallclock timer
    timestate = time;
} 

void WallClock::set_temperature(uint tempval){
    temperature = tempval;
}

u_char WallClock::get_events(void){
    // get event state
    return usr_event;
}

void WallClock::clr_events(void){
    // clear event state
    usr_event = EVENT_INIT;
}

void WallClock::update(uint sequencer_state) {
    // Update wallclock timer state
    bool update_curr_state = false;

    static uint trigger_count = 0;

    if (sequencer_state == UPD_PWRCLK_EV) {

        // trigger handler ( state transition )
        if ( trigger_count == 0 ) {
            
            // State Freeze: no decr when disabled.
            if ( cur_state == STATE_DISABLE) {
                if (new_state == STATE_TRIGGER) {
                    update_curr_state = true;
                }
            // State update: .. update to new state.
            } else {
                update_curr_state = true;
            }
        }

        //* Update current state (+ events)
        if ( update_curr_state) {
            cur_state = new_state;
            usr_event = usr_event | new_state;
        }

        //* trigger handler ( state timeout )
        if ( new_state == STATE_NORMAL) {
            if ( trigger_count > 0 ) {
                trigger_count -= 1;
            } 
        } else {
            trigger_count = 10;
        }
        new_state = STATE_NORMAL;
    }
}

void WallClock::update_clockstate(u_char st) {
    // capture new state ( unconditionally )
    new_state = st;
}

void WallClock::update_wallclock(void){
    // Update wallclock timer value

    if ( strcmp(MQTT_CLIENT_ROLE, "MASTER") == 0 ) {

        //*
        if ( cur_state == STATE_DISABLE ) {
            alarm_enabled = false;
            timestate = WALLCLOCK_INFF;
        } else

        // No state change: Decrement the timer
        if ( cur_state == STATE_NORMAL) { 
            if ( timestate > 0 ) {
                timestate -= 1;
            }
        } else
        
        // trigger 
        if ( cur_state == STATE_TRIGGER) { 
            //*
            if  ( timestate == WALLCLOCK_INFF ) {
                alarm_enabled = true;
                timestate = WALLCLOCK_INIT;
            } else
            //* 
            if ( timestate > WALLCLOCK_LONG ) {
                alarm_enabled = false;
                timestate = WALLCLOCK_SHORT;
            } else 
            //*
            {
                alarm_enabled = true;
                timestate = WALLCLOCK_INIT;
            }
        }
    }
}

