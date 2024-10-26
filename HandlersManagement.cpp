/*BEHAVIOR LAYER: Handlers*/

#include "HandlersManagement.h"

HandlersManagement::HandlersManagement( StateManagement* stateManager, CommunicationManager* communicationManager)
  : stateManager(stateManager), communicationManager(communicationManager) { }

bool HandlersManagement::handleUpdateState(const SensorState& newState) {
  return stateManager->updateSensorState(newState);
}

bool HandlersManagement::handleUpdateThresholds(const ThresholdState& newThresholds) {
  return stateManager->updateThresholds(newThresholds);
}

SensorState HandlersManagement::handleGetState() const {
  return stateManager->getSensorState();
}

HealthStatus HandlersManagement::handleGetHealth() const {
  return stateManager->getHealthStatus();
}

bool HandlersManagement::handleThresholdExceeded(const String& sensorType) {
  return communicationManager->publishEvent("THRESHOLD_EXCEEDED", sensorType);
}

bool HandlersManagement::handleHealthStateChanged(HealthStatus newStatus) {
  return stateManager->updateHealth(newStatus);
}