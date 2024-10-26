/*CONFIGURATION LAYER: Threshold Management*/

#ifndef THRESHOLD_MANAGEMENT_H
#define THRESHOLD_MANAGEMENT_H

#include "StateManagement.h"

class ThresholdManagement {
private:
    StateManagement* stateManager; 

public:
    explicit ThresholdManagement(StateManagement* stateManager);

    // Threshold Configuration
    bool updateThresholds(const ThresholdState& thresholds); 
    bool updateTemperatureThreshold(float max);
    bool updateHumidityThreshold(float max);
    bool updatePressureThreshold(float max);

    // Threshold Queries
    const ThresholdState& getThresholds() const;
};

#endif