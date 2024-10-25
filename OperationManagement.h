/*OPERATION LAYER: Operation Management*/

#ifndef OPERATION_MANAGEMENT_H
#define OPERATION_MANAGEMENT_H

#include "StateManagement.h"
#include "ThresholdManagement.h"
#include "CommunicationManager.h"
#include "SensorDriver.h"

class OperationManagement {
private:
    StateManagement* stateManager;
    ThresholdManagement* thresholdManager;
    CommunicationManager* communicationManager;
    SensorDriver* sensorDriver;
    
    unsigned long lastOperationTime;
    static const unsigned long OPERATION_INTERVAL = 10000; // 10 segundos
    
    bool shouldSendData();

public:
    OperationManagement(
      StateManagement* stateManager, 
      ThresholdManagement* thresholdManager,
      SensorDriver* sensorDriver,
      CommunicationManager* communicationManager
    );

    void executeOperationCycle();
    void scheduleOperations();
};

#endif