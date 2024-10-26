/*BEHAVIOR LAYER: Handlers*/

#ifndef HANDLERS_MANAGEMENT_H
#define HANDLERS_MANAGEMENT_H

#include "StateManagement.h"
#include "CommunicationManager.h"

class HandlersManagement {
private:
    StateManagement* stateManager;
    CommunicationManager* communicationManager;

public:
    HandlersManagement(StateManagement* stateManager, CommunicationManager* communicationManager);

    // Command Handlers
    bool handleUpdateState(const SensorState& newState);
    bool handleUpdateThresholds(const ThresholdState& newThresholds);

    // Query Handlers
    SensorState handleGetState() const;
    HealthStatus handleGetHealth() const;

    // Event Handlers
    bool handleThresholdExceeded(const String& sensorType);
    bool handleHealthStateChanged(HealthStatus newStatus);
};

#endif