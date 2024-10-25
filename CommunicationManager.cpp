/*INTERFACE LAYER: Communication Manager*/

#include "CommunicationManager.h"
#include <ArduinoJson.h>

CommunicationManager::CommunicationManager(StateManagement* stateManager) 
    : stateManager(stateManager), connected(false) { }

bool CommunicationManager::connectWiFi() {

}

void CommunicationManager::sendState() {
    if (!isConnected()) {
        Serial.println("Not connected to WiFi. Cannot send state.");
        return;
    }

    Serial.println("Preparing to send state...");
    
    httpClient.begin(edgeApiUrl);
    httpClient.setTimeout(10000);

    DynamicJsonDocument json(1024);
    const SensorState& state = stateManager->getSensorState();
    
    json["temperature"] = state.temperature;
    json["humidity"] = state.humidity;
    json["pressure"] = state.pressure;

    String jsonString;
    serializeJson(json, jsonString);

    httpClient.addHeader("Content-Type", "application/json");
    int httpResponseCode = httpClient.POST(jsonString);

    if (httpResponseCode > 0) {
        String response = httpClient.getString();
        Serial.println("HTTP Response code: " + String(httpResponseCode));
        Serial.println("Response: " + response);
    } else {
        Serial.println("Error sending state. HTTP Response code: " + String(httpResponseCode));
    }

    httpClient.end();
}

String CommunicationManager::healthStatusToString(HealthStatus status) {
    switch (status) {
        case HealthStatus::HEALTHY:
            return "HEALTHY";
        case HealthStatus::WARNING:
            return "WARNING";
        case HealthStatus::CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

void CommunicationManager::sendHealth() {

}

bool CommunicationManager::publishEvent(const String& eventType, const String& sensorType) {
	
}
