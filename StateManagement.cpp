/* STATE LAYER: State Management */

#include "StateManagement.h"

bool StateManagement::validateSensorState(const SensorState& state) const {
  return (state.temperature >= 0.0f && state.temperature <= 85.0f) &&
         (state.humidity >= 0.0f && state.humidity <= 100.0f) &&
         (state.pressure >= 3.0f && state.pressure <= 255.0f) &&
         (state.gas >= 0.0f && state.gas <= 100.0f);
}

bool StateManagement::validateThresholds(const ThresholdState& thresholds) const {
  return (thresholds.temperatureMax > 0.0f && thresholds.temperatureMax <= 85.0f) &&
         (thresholds.humidityMax > 0.0f && thresholds.humidityMax <= 100.0f) &&
         (thresholds.pressureMax > 3.0f && thresholds.pressureMax <= 255.0f) &&
         (thresholds.gasMax > 0.0f && thresholds.gasMax <= 100.0f);
}

HealthStatus StateManagement::calculateHealthStatus(const SensorState& state, const ThresholdState& thresholds) const {
  if (
      state.temperature > thresholds.temperatureMax ||
      state.humidity > thresholds.humidityMax ||
      state.pressure > thresholds.pressureMax ||
      state.gas      > thresholds.gasMax
    ) {
    return HealthStatus::WARNING;
  }
  return HealthStatus::HEALTHY;
}

/*[State Actions]*/
/*--------------------*/
bool StateManagement::updateSensorState(const SensorState& newState) {
  if (!validateSensorState(newState)) {
    Serial.println("Invalid sensor state.");
    return false;
  }

  store.sensorState = newState;
  store.healthStatus = calculateHealthStatus(newState, store.thresholdState);
  Serial.println("Update Sensor State: True");
  Serial.println("-------------------------------");
  return true;
}

bool StateManagement::updateThresholds(const ThresholdState& newThresholds) {
  if (!validateThresholds(newThresholds)) {
    Serial.println("Invalid thresholds.");
    return false;
  }

  store.thresholdState = newThresholds;
  store.healthStatus = calculateHealthStatus(store.sensorState, newThresholds);
  Serial.println("Thresholds updated.");
  return true;
}

void StateManagement::updateHealth(HealthStatus newStatus) { store.healthStatus = newStatus; }


/*[State Queries]*/
/*--------------------*/
const SensorState& StateManagement::getSensorState() const { return store.sensorState; }
const ThresholdState& StateManagement::getThresholds() const { return store.thresholdState; }
HealthStatus StateManagement::getHealthStatus() const { return store.healthStatus; }