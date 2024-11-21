// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2023 

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

// Chip State Values
typedef struct {
  pin_t pin_scl;
  pin_t pin_sda;
  pin_t pin_vcc;
  pin_t pin_gnd;
  uint32_t temperature_attr;
  uint32_t temperature_threshold_attr;
  uint32_t humidity_attr;
  uint32_t humidity_threshold_attr;
  uint32_t pressure_attr;
  uint32_t pressure_threshold_attr;
  uint32_t altitude_attr;
} chip_state_t;

// Pre-declare Timer Event Handler
static void chip_timer_event(void *user_data);

// I2C Callbacks
bool on_i2c_connect(void *user_data, uint32_t address, bool read) { return true; }

uint8_t on_i2c_read(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;
  static uint8_t read_index = 0;

  switch (read_index) {
    case 0: 
      read_index++;
      return (uint8_t)(attr_read_float(chip->temperature_attr));
    case 1: 
      read_index++;
      return (uint8_t)(attr_read_float(chip->humidity_attr));
    case 2:
      read_index++;
      return (uint8_t)(attr_read_float(chip->pressure_attr));
    case 3: 
      read_index = 0; // Reiniciar el índice para la próxima lectura
      return (uint8_t)(attr_read_float(chip->altitude_attr));
    default:
      return 0;
  }
}

//TODO: Manejar la escritura de datos, por lo pronto devuelve "true"
bool on_i2c_write(void *user_data, uint8_t data) { return true; }

//Opcionalmente se puede manejar la desconexión
void on_i2c_disconnect(void *user_data) { } 

void chip_init() {
  // Chip State Configuration
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  // Setup Chip Pins
  chip->pin_scl  = pin_init("SCL", INPUT_PULLUP);
  chip->pin_sda  = pin_init("SDA", INPUT_PULLUP);
  chip->pin_vcc  = pin_init("VCC", OUTPUT);
  chip->pin_gnd  = pin_init("GND", INPUT_PULLUP);
  pin_write(chip->pin_vcc, HIGH);

  // Setup Chip Attributes for each sensor data and thresholds
  chip->temperature_attr           = attr_init_float("temperature", 30.0);
  chip->temperature_threshold_attr = attr_init_float("temperatureThreshold", 35.0);
  chip->humidity_attr              = attr_init_float("humidity", 30.0);
  chip->humidity_threshold_attr    = attr_init_float("humidityThreshold", 50.0);
  chip->pressure_attr              = attr_init_float("pressure", 70.00);
  chip->pressure_threshold_attr    = attr_init_float("pressureThreshold", 90.0);
  chip->altitude_attr              = attr_init_float("altitude", 220.0);

  // Setup I2C Configuration
  const i2c_config_t i2c_config = {
    .address = 0x76, // Dirección I2C del BME280 
    .scl = chip->pin_scl,
    .sda = chip->pin_sda,
    .connect = on_i2c_connect,
    .read = on_i2c_read,
    .write = on_i2c_write,
    .disconnect = on_i2c_disconnect,
    .user_data = chip,
  };

  // Initialize I2C device
  i2c_init(&i2c_config);

  // Setup Timer Event
  const timer_config_t timer_config = {
    .callback = chip_timer_event,
    .user_data = chip
  };

  // Timer Initialization
  timer_t timer_id = timer_init(&timer_config);
  
  // Timer Start
  timer_start(timer_id, 5000, true);
}

// Timer Event Handler Implementation
void chip_timer_event(void *user_data) {
  chip_state_t *chip = (chip_state_t*)user_data;

  if (pin_read(chip->pin_vcc) && !pin_read(chip->pin_gnd)) {
    float temperature           = attr_read_float(chip->temperature_attr);
    float temperature_threshold = attr_read_float(chip->temperature_threshold_attr);

    float humidity              = attr_read_float(chip->humidity_attr);
    float humidity_threshold    = attr_read_float(chip->humidity_threshold_attr);

    float pressure              = attr_read_float(chip->pressure_attr);
    float pressure_threshold    = attr_read_float(chip->pressure_threshold_attr);

    float altitude              = attr_read_float(chip->altitude_attr);

    if (temperature > temperature_threshold) { 
      printf(
        "Warning: Temperature exceeds threshold! (%.2f°C)\n", 
        temperature
      );
    }
    if (humidity > humidity_threshold) {
      printf(
        "Warning: Humidity exceeds threshold! (%.2f%%)\n",
        humidity
      );
    }
    if (pressure > pressure_threshold) {
      printf(
        "Warning: Pressure exceeds threshold! (%.2f hPa)\n",
        pressure
      );
    }

    printf(
      "Temperature: %.2f°C, Humidity: %.2f%%, Pressure: %.2f hPa, Altitude: %.2f m\n",
      temperature, humidity, pressure, altitude
    );

  } else {
    printf("Sensor is not powered! Please check the power connection.\n");
  }
}
