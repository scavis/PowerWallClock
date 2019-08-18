/**************************************************************************/
/*!
    @file     wallclock.h
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ensure this library description is only included once
#ifndef wallclock_h
#define wallclock_h

// ESP32 arduino libraries
#include <Arduino.h>

// Project libraries
#include "global.h"
#include "custom.h"

class WallClock {
private:

  uint timestate;
  uint cur_state;
  uint new_state;
  uint temperature;

  u_char usr_event;

  bool alarm_enabled;

public:
  WallClock();

  void begin(void);

  bool is_power_active(void);
  bool is_alarm_active(void);
  bool is_clock_active(void);

  void update_clockstate(u_char);

  void update_wallclock(void);

  void update(uint);

  void set_wallclock(uint);

  void set_temperature(uint);

  uint get_wallclock(void);
  uint get_temperature(void);

  u_char get_events(void);
  void clr_events(void);


};


#endif