/**************************************************************************/
/*!
    @file     sequencer.h
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ensure this library description is only included once
#ifndef sequencer_h
#define sequencer_h

// ESP32 arduino libraries
#include <Arduino.h>

// Project libraries
#include "global.h"
#include "custom.h"


class Sequencer {
private:
  uint curr_state;

public:
  Sequencer();

  void begin();
  void increment();
  void update(uint*);
};



#endif