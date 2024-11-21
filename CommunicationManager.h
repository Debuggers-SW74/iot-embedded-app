/* INTERFACE LAYER: Communication Manager */

#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "StateManagement.h"

//MAC ADDRESS
#define MAC_ADDRESS "12:34:56:78:9A:BC" 

// Endpoints
#define BASE_URL "https://fastporte-edge-api.azurewebsites.net/api/v1/device-details"
#define STATE_ENDPOINT      "/state"
#define HEALTH_ENDPOINT     "/health"
#define EVENT_ENDPOINT      "/events"
#define THRESHOLD_ENDPOINT  "/thresholds"
#define CURRENT_TRIP        "/current/trip"
#define REAL_TIME_DATA      "/send-data/real-time"

// WiFi Configuration
#define WIFI_SSID     "Wokwi-GUEST"
#define WIFI_PASSWORD ""

class CommunicationManager {
private:
  String token;
  StateManagement* stateManager;
  HTTPClient httpClient;
  bool connected;

  String constructUrl(const char* endpoint);
  bool sendRequest(const String& url, const String& method, const String& payload);
  String createJsonPayload(const SensorState& state);
  String createHealthPayload(HealthStatus status);
  String createEventPayload(
    const String& eventType,
    const String& sensorType,
    float currentValue,
    float thresholdValue
  );
  
  ThresholdState parseThresholdResponse(const String& response);
  bool getToken(const String& macAddress);

public:
  explicit CommunicationManager(StateManagement* stateManager);
  bool connectWiFi();
  bool isConnected();
    
  bool sendState();              // [POST]:  Send State
  bool sendHealth();             // [POST]:  Send Health
  bool publishEvent(
    const String& eventType, 
    const String& sensorType, 
    float currentValue, 
    float thresholdValue
  );                            // [POST]:  Publish Event

  bool getState();
  bool checkNewThresholds();
  ThresholdState getNewThresholds();

private:
  //Trip Data
  int driverId = 6;
  int truckId = 1;
  
  //extract from edge API endpoint
  int tripId;
  String driverName;
  int supervisorId;
  String supervisorName;

public:
  int getDriverId();
  int getSupervisorId();
  int getTruckId();
  int getTripId();
  String getDriverName();
  String getSupervisorName();

  bool getCurrentTripDetails();

public:
  bool sendRealTimeData();
};

#endif // COMMUNICATION_MANAGER_H