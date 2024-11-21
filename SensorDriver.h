/* BEHAVIOR LAYER: Sensor Driver */

#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include "StateManagement.h"
#include <MQUnifiedsensor.h>

#define BME280_I2C_ADDRESS 0x76 // Dirección I2C del sensor BME280

#define   Board     ("ESP-32")
#define   Pin       (34) 
#define   Threshold (23)
#define   Type      ("MQ-2")
#define   Voltage_Resolution  (5.0)
#define   ADC_Bit_Resolution  (12)
#define   RatioMQ2CleanAir    (9.83)

class SensorDriver {
private:
  /*BME280 Sensor*/
  float temperature;
  float humidity;
  float pressure;
  float altitude;

  void readSensorData();
  void transformData();

public:
  SensorDriver();
  void collectData();
  float getTemperature() const;
  float getHumidity() const;
  float getPressure() const;
  float getAltitude() const;

private:
  /*Gas Sensor MQ2*/
  MQUnifiedsensor* gasSensor;
  float gasValue;

public:
  void initGasSensor();
  void gasCollectData();
  float getGasValue() const;
};

#endif // SENSOR_DRIVER_H