/* BEHAVIOR LAYER: Handlers */

#include "HandlersManagement.h"

HandlersManagement::HandlersManagement(StateManagement* stateManager, CommunicationManager* communicationManager)
  : stateManager(stateManager), communicationManager(communicationManager) {}

bool HandlersManagement::validateStateCommand(const UpdateStateCommand& command) const {
  return stateManager->validateSensorState(command.newState);
}

bool HandlersManagement::validateThresholdCommand(const UpdateThresholdCommand& command) const {
  return stateManager->validateThresholds(command.newThresholds);
}

bool HandlersManagement::shouldPublishEvent(const ThresholdExceededEvent& event) const {
  return event.currentValue > event.thresholdValue;
}

/* Command Handlers */
/*--------------------*/
bool HandlersManagement::handleUpdateState(const UpdateStateCommand& command) {
  if (!validateStateCommand(command)) {
    Serial.println("Invalid state command.");
    return false;
  }

  bool updated = stateManager->updateSensorState(command.newState);
  if (updated) {
    Serial.println("Sensor state updated successfully.");
    communicationManager->sendState();
  } else { Serial.println("Failed to update sensor state."); }

  return updated;
}

bool HandlersManagement::handleUpdateThresholds(const UpdateThresholdCommand& command) {
  if (!validateThresholdCommand(command)) {
    Serial.println("Invalid threshold command.");
    return false;
  }

  bool updated = stateManager->updateThresholds(command.newThresholds);
  if (updated) {
    Serial.println("Thresholds updated successfully.");
    communicationManager->sendHealth();
  } else {
    Serial.println("Failed to update thresholds.");
  }
  
  return updated;
}

bool HandlersManagement::handleHealthStateChange(HealthStatus newStatus) {
  stateManager->updateHealth(newStatus); 
  communicationManager->sendHealth();
  return true;
}


/* Query Handlers */
/*--------------------*/
SensorState HandlersManagement::handleGetState(const StateQuery& query) const {
  SensorState state = stateManager->getSensorState();
  if (query.includeHealth) { communicationManager->sendHealth(); }
  return state;
}
HealthStatus HandlersManagement::handleGetHealth() const { return stateManager->getHealthStatus(); }


/* Event Handlers */
/*--------------------*/
bool HandlersManagement::handleThresholdExceeded(const ThresholdExceededEvent& event) {
  if (!shouldPublishEvent(event)) { return false; }
  return communicationManager->publishEvent(
    "THRESHOLD_EXCEEDED", 
    event.sensorType,
    event.currentValue,
    event.thresholdValue
  );
}

bool HandlersManagement::handleConnectionStateChanged(bool connected) {
  if (connected) { return communicationManager->sendState(); }
  return true;
}