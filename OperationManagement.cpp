#include "OperationManagement.h"

OperationManagement::OperationManagement(
  SensorDriver* sensorDriver, 
  CommunicationManager* commManager,
  StateManagement* stateManager,
  HandlersManagement* handlersManagement,
  RequestManager* requestManager,
  ThresholdManagement* thresholdManagement,
  ActuatorDriver* actuatorDriver
)
  : sensorDriver(sensorDriver), 
    communicationManager(commManager), 
    stateManager(stateManager), 
    handlersManagement(handlersManagement),
    requestManager(requestManager),
    thresholdManagement(thresholdManagement),
    actuatorDriver(actuatorDriver),
    lastExecutionTime(0) { }


void OperationManagement::operationEngine() {
  sensorDriver->collectData();      //BME280
  sensorDriver->gasCollectData();   //MQ2

  SensorState currentState = {
    .temperature = sensorDriver->getTemperature(),
    .humidity = sensorDriver->getHumidity(),
    .pressure = sensorDriver->getPressure(),
    .gas = sensorDriver->getGasValue()
  };

  if (communicationManager->isConnected()) {
    requestManager->updateState(currentState);
    communicationManager->sendState();
    communicationManager->sendHealth();

    if (millis() - lastExecutionTime >= 520) { 

      Serial.println("Enviando datos al servidor...");
      if (communicationManager->sendRealTimeData()) {
        Serial.println("Datos enviados correctamente.");
      } else {
        Serial.println("Error al enviar los datos.");
      }

      lastExecutionTime = millis(); // Reiniciar el temporizador
    }
  }
  else {
    Serial.println("Retrying to connect to Wifi...");
    communicationManager->connectWiFi();
    Serial.println("-------------------------------");
  }

  if (communicationManager->checkNewThresholds()) {
    ThresholdState newThresholds = communicationManager->getNewThresholds();
    if (thresholdManagement->updateThresholds(newThresholds)) {
      Serial.println("Thresholds updated successfully.");
    } else {
      Serial.println("Failed to update thresholds.");
    }
  }


  /*check thresholds and launch events*/
  /*----------------------------------*/

  const SensorState& currentStateFromStateManager = stateManager->getSensorState();
  ThresholdState currentThresholds = stateManager->getThresholds();
  bool thresholdExceeded = false;

  if (currentStateFromStateManager.temperature > currentThresholds.temperatureMax) {
    ThresholdExceededEvent event = {
      .sensorType = "SENSOR_TEMPERATURE",
      .currentValue = currentStateFromStateManager.temperature,
      .thresholdValue = currentThresholds.temperatureMax,
      .timestamp = millis()
    };
    handlersManagement->handleThresholdExceeded(event);
    thresholdExceeded = true;
  }

  if (currentStateFromStateManager.humidity > currentThresholds.humidityMax) {
    ThresholdExceededEvent event = {
      .sensorType = "SENSOR_HUMIDITY",
      .currentValue = currentStateFromStateManager.humidity,
      .thresholdValue = currentThresholds.humidityMax,
      .timestamp = millis()
    };
    handlersManagement->handleThresholdExceeded(event);
    thresholdExceeded = true;
  }

  if (currentStateFromStateManager.pressure > currentThresholds.pressureMax) {
    ThresholdExceededEvent event = {
      .sensorType = "SENSOR_PRESSURE",
      .currentValue = currentStateFromStateManager.pressure,
      .thresholdValue = currentThresholds.pressureMax,
      .timestamp = millis()
    };
    handlersManagement->handleThresholdExceeded(event);
    thresholdExceeded = true;
  }

  if (currentStateFromStateManager.gas > currentThresholds.gasMax) {
    ThresholdExceededEvent event = {
      .sensorType = "SENSOR_GAS",
      .currentValue = currentStateFromStateManager.gas,
      .thresholdValue = currentThresholds.gasMax,
      .timestamp = millis()
    };
    handlersManagement->handleThresholdExceeded(event);
    thresholdExceeded = true;
  }



  /*Control of LEDs and buzzer using ActuatorDriver*/
  /*----------------------------------*/
  if (thresholdExceeded) {
    actuatorDriver->turnOffGreenLed();  // Turn off the green LED
    actuatorDriver->turnOnRedLed();     // Turn on the red LED
    //actuatorDriver->activateBuzzer();   // Activate the buzzer
  } else {
    actuatorDriver->turnOnGreenLed();   // Turn on the green LED
    actuatorDriver->turnOffRedLed();    // Turn off the red LED
    actuatorDriver->deactivateBuzzer(); // Deactivate the buzzer
  }
} 

void OperationManagement::run() {
  while (true) {
    unsigned long currentMillis = millis();
        
    if (currentMillis - lastExecutionTime >= interval) { //Get the data in intervals
      lastExecutionTime = currentMillis;
      operationEngine();
    }
    //delay(10);
    if (millis() % 10 == 0) {
      yield(); // Cede el control a otros procesos
    }
  }
}