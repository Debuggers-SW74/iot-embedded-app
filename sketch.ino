#include <Arduino.h>
#include "StateManagement.h"
#include "SensorDriver.h"
#include "OperationManagement.h"
#include "CommunicationManager.h"
#include "HandlersManagement.h"
#include "ThresholdManagement.h"
#include "RequestManager.h"
#include "ActuatorDriver.h"

SensorDriver sensorDriver;
StateManagement stateManager;
RequestManager requestManager(&stateManager);
CommunicationManager communicationManager(&stateManager);
HandlersManagement handlersManagement(&stateManager, &communicationManager);
ThresholdManagement thresholdManagement(&stateManager);
ActuatorDriver actuatorDriver;

OperationManagement operationManagement(
  &sensorDriver, &communicationManager, 
  &stateManager, &handlersManagement,
  &requestManager, &thresholdManagement,
  &actuatorDriver
);

void setup() {
  Serial.begin(115200);
  //delay(500);
    
  if (communicationManager.connectWiFi()) {
     ThresholdState initialThresholds = communicationManager.getNewThresholds();
      Serial.println("Initial thresholds obtained: " +
                     String(initialThresholds.temperatureMax) + ", " +
                     String(initialThresholds.humidityMax) + ", " +
                     String(initialThresholds.pressureMax) + ", " +
                     String(initialThresholds.gasMax));
    if (thresholdManagement.updateThresholds(initialThresholds)) {
      Serial.println("Initial thresholds set correctly.");
    } else {
      Serial.println("Error setting initial thresholds.");
    }

    sensorDriver.gasCollectData();
    sensorDriver.initGasSensor();
    sensorDriver.collectData(); 
    actuatorDriver.init();
  } else {
    Serial.println("Error connecting to WiFi.");
  }
  Serial.println("-------------------------------");
}

void loop() { operationManagement.run(); }