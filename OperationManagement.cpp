/*OPERATION LAYER: Operation Management*/


#include "OperationManagement.h"

OperationManagement::OperationManagement(
    StateManagement* stateManager, 
    ThresholdManagement* thresholdManager,
    SensorDriver* sensorDriver,
    CommunicationManager* communicationManager)
    : stateManager(stateManager)
    , thresholdManager(thresholdManager)
    , sensorDriver(sensorDriver)
    , communicationManager(communicationManager)
    , lastOperationTime(0) {
}

void OperationManagement::executeOperationCycle() {
  SensorState newState = sensorDriver->collectData();
    
  if (stateManager->updateSensorState(newState)) {
    if (shouldSendData()) {
      communicationManager->sendState();
      lastOperationTime = millis();
    }
  }
}

void OperationManagement::scheduleOperations() {
  unsigned long currentTime = millis();
    
  if (currentTime - lastOperationTime >= OPERATION_INTERVAL) {
    executeOperationCycle();
  }
}
bool OperationManagement::shouldSendData() {
  const SensorState& currentState = stateManager->getSensorState();
  const ThresholdState& thresholds = thresholdManager->getThresholds();
    
  return (currentState.temperature > thresholds.temperatureMax ||
    currentState.humidity > thresholds.humidityMax ||
    currentState.pressure > thresholds.pressureMax);
}
