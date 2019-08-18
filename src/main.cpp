/**************************************************************************/
/*!
    @file     main.cpp
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// project related libraries
#include <Arduino.h>

#include "wallclock.h"
#include "userinterface.h"
#include "netconnection.h"
#include "sequencer.h"

#include "custom.h"

WallClock wc = WallClock();
Sequencer se = Sequencer();

NetConnection nc = NetConnection(&wc);
UserInterface ui = UserInterface(&wc);


void setup() {
  // setup procedure ( runs at boot sequence only).

  Serial.begin(SERSPEED);
  Serial.println("");
  Serial.println("** Usb debug port initialized. **");

  ui.begin();
  Serial.println("** User interface initialized. **");

  nc.begin();
  Serial.println("** Network connection initialized. **");

  wc.begin();
  Serial.println("** PowerWallClock initialized. **");

  se.begin();
  Serial.println("** System sequencer initialized. **");

}

void loop() {
  // loop procedure ( runs/recalls function endless).
  u_int seq_state;

  se.update(&seq_state);

  wc.update(seq_state);
  
  ui.update(seq_state);
  nc.update(seq_state);

  nc.loop();

}