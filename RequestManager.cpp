/*INTERFACE LAYER: Request Manager*/

#include "RequestManager.h"

RequestManager::RequestManager(StateManagement* stateManager) 
  : stateManager(stateManager) { }

SensorState RequestManager::getState() const {
  return stateManager->getSensorState();
}

bool RequestManager::updateState(const SensorState& newState) {
  //TO DO: El sensor BME280 lee datos, pero se podría actualizar algún dato manualmente 
  return stateManager->updateSensorState(newState);
}

bool RequestManager::updateThresholds(const ThresholdState& newThresholds) {
  return stateManager->updateThresholds(newThresholds);
}

HealthStatus RequestManager::getHealth() const {
  return stateManager-> getHealthStatus();
}
