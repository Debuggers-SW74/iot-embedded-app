/*INTERFACE LAYER: Communication Manager*/

#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "StateManagement.h"

#define WIFI_SSID     "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define ENDPOINT_URL  "https://iot-edge-api-fastporte.free.beeceptor.com/api/v1/state"

class CommunicationManager {
private:
  String edgeApiUrl = ENDPOINT_URL;
  StateManagement* stateManager;
  HTTPClient httpClient;
  bool connected;

public:
  explicit CommunicationManager(StateManagement* stateManager);
    
  bool connectWiFi();
  void sendState();  //POST Send State
  void sendHealth(); //POST Send Health
  bool isConnected() const { return connected; }
  bool publishEvent(const String& eventType, const String& sensorType); 
    
  static String healthStatusToString(HealthStatus status); // Convertir HealthStatus a String
};

#endif