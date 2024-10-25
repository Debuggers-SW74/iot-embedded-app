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

void chip_init() {
  // Chip State Configuration
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  // Setup Chip Pins
  chip->pin_scl  = pin_init("SCL", INPUT_PULLUP);
  chip->pin_sda  = pin_init("SDA", INPUT_PULLUP);
  chip->pin_vcc  = pin_init("VCC", INPUT_PULLDOWN);
  chip->pin_gnd  = pin_init("GND", INPUT_PULLUP);

  // Setup Chip Attributes for each sensor data and thresholds
  chip->temperature_attr           = attr_init_float("temperature", 25.0);           // Default temperature 
  chip->temperature_threshold_attr = attr_init_float("temperatureThreshold", 35.0);  // Default threshold

  chip->humidity_attr              = attr_init_float("humidity", 30.0);              // Default humidity 
  chip->humidity_threshold_attr    = attr_init_float("humidityThreshold", 60.0);     // Default threshold

  chip->pressure_attr              = attr_init_float("pressure", 1000.25);           // Default pressure 
  chip->pressure_threshold_attr    = attr_init_float("pressureThreshold", 1020.0);   // Default threshold

  chip->altitude_attr              = attr_init_float("altitude", 600.0);             // Default altitude 

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

  // Verificar que el sensor esté alimentado correctamente
  if (pin_read(chip->pin_vcc) && !pin_read(chip->pin_gnd)) {

    // Leer los valores de los atributos (sensor values)
    float temperature           = attr_read_float(chip->temperature_attr);
    float temperature_threshold = attr_read_float(chip->temperature_threshold_attr);

    float humidity              = attr_read_float(chip->humidity_attr);
    float humidity_threshold    = attr_read_float(chip->humidity_threshold_attr);

    float pressure              = attr_read_float(chip->pressure_attr);
    float pressure_threshold    = attr_read_float(chip->pressure_threshold_attr);

    float altitude              = attr_read_float(chip->altitude_attr);

    // Verificar los umbrales para cada parámetro
    if (temperature > temperature_threshold) { 
      printf("Warning: Temperature exceeds threshold! (%.2f°C)\n", temperature);
    }
    if (humidity > humidity_threshold) {
      printf("Warning: Humidity exceeds threshold! (%.2f%%)\n", humidity);
    }
    if (pressure > pressure_threshold) {
      printf("Warning: Pressure exceeds threshold! (%.2f hPa)\n", pressure);
    }

    // Salida simulada para los datos del sensor
    printf("Temperature: %.2f°C, Humidity: %.2f%%, Pressure: %.2f hPa, Altitude: %.2f m\n", temperature, humidity, pressure, altitude);
  } else {
    // Si no hay alimentación, el sensor no funciona
    printf("Sensor is not powered! Please check the power connection.\n");
  }
}