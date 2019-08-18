// ensure this library description is only included once
#ifndef Accelerometer_h
#define Accelerometer_h

#include "Adafruit_ADXL345_U.h"
#include "Adafruit_Sensor.h"


class Accelerometer: public Adafruit_ADXL345_Unified {
private:
    int z_accel;
public:
    Accelerometer();

    void begin(void);

    bool is_triggered(void);

    void display_info(void);
};

#endif