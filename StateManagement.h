/*STATE LAYER: State Management*/

#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include <Arduino.h>

struct SensorState {
    float temperature;
    float humidity;
    float pressure;
    float altitude;
};

struct ThresholdState {
    float temperatureMin, temperatureMax;
    float humidityMin, humidityMax;
    float pressureMin, pressureMax;
};

enum class HealthStatus {
    HEALTHY,
    WARNING,
    CRITICAL
};

class StateManagement {
private:
    StateManagement() = default;
    static StateManagement* instance;
public:
    static StateManagement* getInstance();

    //State Store
    struct StateStore {
        SensorState sensorState;
        ThresholdState thresholdState;
        HealthStatus healthStatus;
    } store;

    //State Actions
    bool updateSensorState(const SensorState& newState);
    bool updateThresholds(const ThresholdState& newThresholds);
    bool updateHealth(HealthStatus newStatus);

    const SensorState& getSensorState() const;
    const ThresholdState& getThresholds() const;
    HealthStatus getHealthStatus() const;
};

#endif