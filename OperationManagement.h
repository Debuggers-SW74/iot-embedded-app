/* OPERATION LAYER: Operation Management */

#ifndef OPERATION_MANAGEMENT_H
#define OPERATION_MANAGEMENT_H

#include "SensorDriver.h"
#include "CommunicationManager.h" 
#include "StateManagement.h"
#include "HandlersManagement.h"
#include "ThresholdManagement.h"
#include "RequestManager.h"
#include "ActuatorDriver.h"
#include <Arduino.h>

class OperationManagement {
private:
  SensorDriver* sensorDriver;
  CommunicationManager* communicationManager;
  StateManagement* stateManager;
  HandlersManagement* handlersManagement;
  RequestManager* requestManager;
  ThresholdManagement* thresholdManagement;
  ActuatorDriver* actuatorDriver;
    
  unsigned long lastExecutionTime;
  //const unsigned long interval = 5000; // 5 segundos para la recolección de datos
  const unsigned long interval = 2000;

  //Operation Engine
  void operationEngine();

public:
  explicit OperationManagement(
    SensorDriver* sensorDriver, 
    CommunicationManager* commManager,
    StateManagement* stateManager,
    HandlersManagement* handlersManagement,
    RequestManager* requestManager,
    ThresholdManagement* thresholdManagement,
    ActuatorDriver* actuatorDriver
  );

  void run();
};

#endif // OPERATION_MANAGEMENT_H