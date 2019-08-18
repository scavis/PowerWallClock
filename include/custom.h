/**************************************************************************/
/*!
    @file     custom.h
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ensure this library description is only included once
#ifndef custom_h
#define custom_h

#define EMULATE_PWRCTRL 1

//* Modify network configuration
#define WIFI_SSID "ko-lab-net"
#define WIFI_PASSWORD "<WIFI_PASSWORD>"

#define MQTT_SERVER_PORT 1883
#define MQTT_SERVER_IP "10.90.154.111"


#define MQTT_NODE 0

//* Nodes definition
#if MQTT_NODE == 0
    //* Master MQTT Node Details
    #define MQTT_CLIENT_ID "WCLK_MSTR"
    #define MQTT_CLIENT_ROLE "MASTER"

#elif MQTT_NODE == 1
    //* Client0 MQTT Node Details
    #define MQTT_CLIENT_ID "WCLK_SLV0"
    #define MQTT_CLIENT_ROLE "SLAVE"

#elif MQTT_NODE == 2
    //* Client1 MQTT Node Details
    #define MQTT_CLIENT_ID "WCLK_SLV1"
    #define MQTT_CLIENT_ROLE "SLAVE"

#elif MQTT_NODE == 3
    //* Client2 MQTT Node Details
    #define MQTT_CLIENT_ID "WCLK_SLV2"
    #define MQTT_CLIENT_ROLE "SLAVE"

#elif MQTT_NODE == 4
    //* Client3 MQTT Node Details
    #define MQTT_CLIENT_ID "WCLK_SLV3"
    #define MQTT_CLIENT_ROLE "SLAVE"

#elif MQTT_NODE == 5
    //* Client4 MQTT Node Details
    #define MQTT_CLIENT_ID "WCLK_SLV4"
    #define MQTT_CLIENT_ROLE "SLAVE"

#endif

//*
#define MQTT_MSG_POWER_ON "on"
#define MQTT_MSG_POWER_OFF "off"

#define MQTT_MSG_TRIGGER "trigger"
#define MQTT_MSG_DISABLE "disable"

#define MQTT_MSG_TOPIC_POWER "cmnd/mainpower/POWER"
#define MQTT_MSG_TOPIC_TIMER "cmnd/mainpower/POWER_TIMER"
#define MQTT_MSG_TOPIC_UMODE "cmnd/mainpower/POWER_UMODE"
#define MQTT_MSG_TOPIC_TEMP "cmnd/mainpower/POWER_TEMP"

#endif