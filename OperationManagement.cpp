/*OPERATION LAYER: Operation Management*/


#include "OperationManagement.h"

OperationManagement::OperationManagement() {
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

}
