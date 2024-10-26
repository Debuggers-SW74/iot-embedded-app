/*STATE LAYER: State Management*/

#include "StateManagement.h"

StateManagement* StateManagement::instance = nullptr;

StateManagement* StateManagement::getInstance() {
  if (!instance) {
    instance = new StateManagement();
  }
  return instance;
}

bool StateManagement::updateSensorState(const SensorState& newState) {
    store.sensorState = newState;
    
    // Actualizar el estado de salud o HealthStatus basado con los Thresholds
    store.healthStatus = 
        (newState.temperature > store.thresholdState.temperatureMax ||
         newState.humidity > store.thresholdState.humidityMax ||
         newState.pressure > store.thresholdState.pressureMax) 
        ? HealthStatus::WARNING : HealthStatus::HEALTHY;
        
    return true;
}

bool StateManagement::updateThresholds(const ThresholdState& newThresholds) {
  store.thresholdState = newThresholds;
  return true;
}

const SensorState& StateManagement::getSensorState() const {
  return store.sensorState;
}

const ThresholdState& StateManagement::getThresholds() const {
  return store.thresholdState;
}

HealthStatus StateManagement::getHealthStatus() const {
  return store.healthStatus;
}