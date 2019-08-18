/**************************************************************************/
/*!
    @file     netconnection.h
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ensure this library description is only included once
#ifndef netconnection_h
#define setconnection_h

// ESP32 arduino libraries
#include <Arduino.h>
#include <WiFi.h>

// MQTT libarires
#include <PubSubClient.h>

// Project libraries
#include "wallclock.h"

#include "global.h"
#include "custom.h"

// ==================================================================
class NetConnection {
private:
  WiFiClient espClient;
  PubSubClient client;

  WallClock* pwrcs;

  bool power_state;

  void reconnect(void);

  void send_power_state(void);

  void mqtt_callback(char*, byte*, uint);

public:
  NetConnection(WallClock*);

  void begin();

  void update(uint);

  void loop();

};


#endif