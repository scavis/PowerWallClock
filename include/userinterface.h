/**************************************************************************/
/*!
    @file     userinterface.h
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ensure this library description is only included once
#ifndef userinterface_h
#define userinterface_h

// ESP32 arduino libraries
#include <Arduino.h>

// include Fri3d badge libraries
#include <Fri3dLegs.h>
#include <Fri3dMatrix.h>
#include <Fri3dBuzzer.h>
#include <Fri3dButtons.h>

// include custom Accelerometer lib
#include <Accelerometer.h>

#include "wallclock.h"

class UserInterface {
private:
  
  Fri3dButtons buttons = Fri3dButtons();
  Fri3dBuzzer buzzer = Fri3dBuzzer();
  Fri3dMatrix matrix= Fri3dMatrix();

  Accelerometer accel = Accelerometer();

  WallClock* pwrcs;

  uint wallclock;
  uint usermode;

  bool alarm_enabled;

  void set_usermode_nudge(void);
  void set_usermode_button(void);
  void set_wallclock(void);
  void set_sound(void);
  void set_matrix(void);
  void set_temperature(void);
  
public:

  UserInterface(WallClock*);

  void begin();
  void update(uint);

};


#endif