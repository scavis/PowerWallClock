/**************************************************************************/
/*!
    @file     netconnection.cpp
    @author   Scavis (@hacker Space: Ko-Lab)
    @license  BSD (see license.txt)

    @section  HISTORY
    
    v1.0 - First release
*/
/**************************************************************************/

// ESP32 arduino libraries
#include <Arduino.h>
#include <WiFi.h>

// MQTT libraries
#include <PubSubClient.h>

// Project libraries
#include "netconnection.h"
#include "wallclock.h"

#include "global.h"
#include "custom.h"


NetConnection::NetConnection(WallClock* pcs) {
    // NetConnection constructor
    pwrcs = pcs;
}

void NetConnection::begin() {
    // NetConnection initializer
    
    // Create wifi connection
    Serial.println("Connect to the WIFI network");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Communicate connection details
    for( u_char cnt = 0; cnt < 50; cnt = cnt + 1 ) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("");
            break;
        }
        sleep(1);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println(WiFi.localIP());
    
    // Add network details (MQTT broker connection)
    client.setClient(espClient);
    
    // Add broker details (MQTT broker connection)
    client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);

    // Add callback functions (MQTT broker connection)
    client.setCallback([this](char* topic, byte* payload, unsigned int length) 
                       {this->mqtt_callback(topic, payload, length);});
}

void NetConnection::loop(void) {
    // Loop function to handle pending callbacks
    client.loop();
}

void NetConnection::mqtt_callback(char* topic, byte* message, unsigned int length) {
    // MQTT generic callback function

    message[length] = '\0';
    String payloadStr((char*)message);

    //* Client Nodes receive the latest timestamps
    if ( strcmp( MQTT_CLIENT_ROLE, "MASTER") != 0) {
        if ( strcmp(topic, MQTT_MSG_TOPIC_TIMER) == 0 ) {
            pwrcs->set_wallclock((unsigned int)payloadStr.toInt() );
            //Serial.print("[Client] Received timer feedback!!!");
        }
    }

    //* Master Node receives the: (trigger, disable state)
    if ( strcmp( MQTT_CLIENT_ROLE, "MASTER") == 0 ) {
        if ( strcmp(topic, MQTT_MSG_TOPIC_UMODE) == 0 ) {
            //*
            if ( payloadStr == MQTT_MSG_TRIGGER ) {
                pwrcs->update_clockstate(STATE_TRIGGER);
                //Serial.print("[Master] Received trigger feedback!!!");
            } else 
            //*
            if ( payloadStr == MQTT_MSG_DISABLE ) {
                pwrcs->update_clockstate(STATE_DISABLE);
                //Serial.print("[Master] Received disable feedback!!!");
            } 
        }
    }
}

void NetConnection::send_power_state(void) {
    //*

    bool active = pwrcs->is_power_active();

    String timerMsg = String(pwrcs->get_wallclock());
    String tempMsg = String(pwrcs->get_temperature());

    u_int userevents = pwrcs->get_events();
    pwrcs->clr_events();

    //* Client Nodes only send out the: ( controls )
    if ( strcmp( MQTT_CLIENT_ROLE, "MASTER") != 0 ) {
        //*
        if ((userevents & STATE_TRIGGER) == STATE_TRIGGER) {
            client.publish(MQTT_MSG_TOPIC_UMODE, MQTT_MSG_TRIGGER );
            //Serial.print("[Client] Send trigger state!!!!");
        }
        //*
        if ((userevents & STATE_DISABLE) == STATE_DISABLE) {
            client.publish(MQTT_MSG_TOPIC_UMODE, MQTT_MSG_DISABLE );
            //Serial.print("[Client] Send disable state!!!!");
        }
    }

    //* Master Node sends out the: ( time, phy power control )
    if ( strcmp( MQTT_CLIENT_ROLE, "MASTER") == 0 ) {
        if (active) {
            //* Set power on state ( when not in emulated mode )
            if ( EMULATE_PWRCTRL == 0) {
                client.publish(MQTT_MSG_TOPIC_POWER, MQTT_MSG_POWER_ON);
            }
            //Serial.print("[Server] Send poweron state!!!!");
        } else {
            //* disable power on state (when not in emulated mode)
            if ( EMULATE_PWRCTRL == 0) {
                client.publish(MQTT_MSG_TOPIC_POWER, MQTT_MSG_POWER_OFF);
            }
            //Serial.print("[Server] Send poweroff state!!!!");
        }

        client.publish(MQTT_MSG_TOPIC_TIMER, timerMsg.c_str() );
        //Serial.print("[Server] Send Timer value!!!!");
    }

    //* Master and client nodes (temperature and user presence)
    client.publish(MQTT_MSG_TOPIC_TEMP, tempMsg.c_str());
    //Serial.print("Temperature message: "); Serial.print(tempMsg); Serial.print("\n");

}

void NetConnection::reconnect(void) {
    //* (re)connect and subscribe to all channels (when MQTT connection down)
    if (WiFi.isConnected()) {
        if (!client.connected()) {
            if (client.connect(MQTT_CLIENT_ID)) {
                client.subscribe(MQTT_MSG_TOPIC_POWER);
                client.subscribe(MQTT_MSG_TOPIC_TIMER);
                client.subscribe(MQTT_MSG_TOPIC_UMODE);
                client.subscribe(MQTT_MSG_TOPIC_TEMP);
                Serial.print("[Node] MQTT Subscriptions done.");
            }
        }
    } else {
        WiFi.reconnect();
    }
}

void NetConnection::update(unsigned int state) {
    //* update all system functions (system state + power state)
    
    //* Keep MQTT connection alive
    if ((state & UPD_RECONN_EV ) == UPD_RECONN_EV) {
        reconnect();
    }
    //* Share system state and set power state
    if ((state & UPD_SENDMSG_EV ) == UPD_SENDMSG_EV) {
        send_power_state();
    }
}