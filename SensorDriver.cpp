/*COMPONENT LAYER: Sensor Driver*/

#include "SensorDriver.h"

SensorDriver::SensorDriver() {
    // Configuración del Sensor BME280
    bme.begin(0x76);
    bme.setSampling(
      Adafruit_BME280::MODE_FORCED,     // Modo de bajo consumo
      Adafruit_BME280::SAMPLING_X1,     // Temperatura
      Adafruit_BME280::SAMPLING_X1,     // Presión
      Adafruit_BME280::SAMPLING_X1,     // Humedad
      Adafruit_BME280::FILTER_OFF       // Sin filtrado
    );
}

SensorState SensorDriver::collectData() {

}
