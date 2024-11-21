/* BEHAVIOR LAYER: Handlers */

#ifndef HANDLERS_MANAGEMENT_H
#define HANDLERS_MANAGEMENT_H

#include "StateManagement.h"
#include "CommunicationManager.h"

// Commands
struct UpdateStateCommand { SensorState newState; };
struct UpdateThresholdCommand { ThresholdState newThresholds; };

// Queries
struct StateQuery { bool includeHealth; };

// Events 
struct ThresholdExceededEvent {
  String sensorType;
  float currentValue;
  float thresholdValue;
  unsigned long timestamp;
};

class HandlersManagement {
private:
  StateManagement* stateManager;
  CommunicationManager* communicationManager;

  bool validateStateCommand(const UpdateStateCommand& command) const;
  bool validateThresholdCommand(const UpdateThresholdCommand& command) const;
  bool shouldPublishEvent(const ThresholdExceededEvent& event) const;

public:
  HandlersManagement(StateManagement* stateManager, CommunicationManager* communicationManager);

  // Command Handlers
  bool handleUpdateState(const UpdateStateCommand& command);
  bool handleUpdateThresholds(const UpdateThresholdCommand& command);
  bool handleHealthStateChange(HealthStatus newStatus);

  // Query Handlers
  SensorState handleGetState(const StateQuery& query) const;
  HealthStatus handleGetHealth() const;

  // Event Handlers
  bool handleThresholdExceeded(const ThresholdExceededEvent& event);
  bool handleConnectionStateChanged(bool connected);
};

#endif // HANDLERS_MANAGEMENT_H