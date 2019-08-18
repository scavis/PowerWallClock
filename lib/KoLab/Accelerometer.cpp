#include "Accelerometer.h"
#include "Arduino.h"

Accelerometer::Accelerometer() {
    Adafruit_ADXL345_Unified(12345);
    z_accel = 0;
}

void Accelerometer::begin(void) {
    //https://www.controllerstech.com/adxl345-single-and-double-tap-functions/
    Adafruit_ADXL345_Unified::begin();

    // set power mode to always in measurement mode!!
    writeRegister(ADXL345_REG_POWER_CTL, 0x00);
    writeRegister(ADXL345_REG_POWER_CTL, 0x08);

    writeRegister(ADXL345_REG_TAP_AXES, 0x1);
    writeRegister(ADXL345_REG_THRESH_TAP,40);
    writeRegister(ADXL345_REG_DUR, 32);  
    writeRegister(ADXL345_REG_LATENT, 80); 
    writeRegister(ADXL345_REG_WINDOW, 240);
    writeRegister(ADXL345_REG_INT_MAP, 0x0); 

    setRange(ADXL345_RANGE_16_G);
    setDataRate(ADXL345_DATARATE_12_5_HZ);

}

bool Accelerometer::is_triggered(void) {
    // https://www.i2cdevlib.com/devices/adxl345#registers

    bool triggered = false;

    int interruptSource = readRegister(ADXL345_REG_INT_SOURCE);

    // when a single tab is detected ...
    if ( (interruptSource>>5)& 0x01  ) {
        Serial.println("### DOUBLE_TAP");
        triggered = true;
    } else
    // when a double tap is detected also a single tap is detected. 
    // we use an else here so that we only print the double tap 
    if ( (interruptSource>>6)& 0x01  ) {
        Serial.println("### SINGLE_TAP");
        triggered = true;
    }
    //}

    //Get a new sensor event
    sensors_event_t event; 
    getEvent(&event);

    // Display the results (acceleration is measured in m/s^2)
    //Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
    //Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
    //Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.print("m/s^2 ");

    if ( event.acceleration.z < ( z_accel - 10) ) triggered = true;
    if ( event.acceleration.z > ( z_accel + 10) ) triggered = true;

    if (triggered == false) {
        z_accel = (z_accel + event.acceleration.z ) /2;
    }

    //Serial.print(" IT:  "); Serial.print(interruptTrigger); 
    //Serial.print(" V:  "); Serial.print(z_accel); Serial.print("\n");

    return triggered;
}

void Accelerometer::display_info(void) {


}
