/*COMPONENT LAYER: Sensor Driver*/

#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "StateManagement.h"

class SensorDriver {
private:
    Adafruit_BME280 bme;
    SensorState transformData(float temp, float hum, float press, float alt);

public:
    explicit SensorDriver();
    SensorState collectData();
};

#endif