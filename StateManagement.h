/* STATE LAYER: State Management */

#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include <Arduino.h>


struct SensorState {
  float temperature;
  float humidity;
  float pressure;
  float altitude;
  float gas;
};

struct ThresholdState {
  float temperatureMax;
  float humidityMax;
  float pressureMax;
  float gasMax;
};

enum class HealthStatus {
  HEALTHY,
  WARNING,
  CRITICAL
};

// [State Store]
struct StateStore { 
  SensorState sensorState;
  ThresholdState thresholdState;
  HealthStatus healthStatus;
};

class StateManagement {
private:
  StateStore store;
  HealthStatus calculateHealthStatus(const SensorState& state, const ThresholdState& thresholds) const;

public:
  StateManagement() = default;

  // [State Actions]
  bool updateSensorState(const SensorState& newState);
  bool updateThresholds(const ThresholdState& newThresholds);
  void updateHealth(HealthStatus newStatus);

  // [State Queries]
  const SensorState& getSensorState() const;
  const ThresholdState& getThresholds() const;
  HealthStatus getHealthStatus() const;

public:
  bool validateSensorState(const SensorState& state) const;
  bool validateThresholds(const ThresholdState& thresholds) const;
};

#endif // STATE_MANAGEMENT_H