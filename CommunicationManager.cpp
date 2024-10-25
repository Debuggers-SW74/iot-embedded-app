/*INTERFACE LAYER: Communication Manager*/

#include "CommunicationManager.h"
#include <ArduinoJson.h>

CommunicationManager::CommunicationManager(StateManagement* stateManager) 
    : stateManager(stateManager), connected(false) { }

bool CommunicationManager::connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi...");

    int retries = 20;
    while (WiFi.status() != WL_CONNECTED && retries > 0) {
        delay(500);
        Serial.print(".");
        retries--;
    }

    connected = (WiFi.status() == WL_CONNECTED);

    if (connected) {
        Serial.println(" Connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("Failed to connect to WiFi.");
        return false;
    }
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
	if (!isConnected()) {
        Serial.println("Not connected to WiFi. Cannot send health status.");
        return;
    }

    Serial.println("Preparing to send health status...");
    httpClient.begin(edgeApiUrl);

    DynamicJsonDocument json(256);
    json["health"] = healthStatusToString(stateManager->getHealthStatus());

    String jsonString;
    serializeJson(json, jsonString);

    httpClient.addHeader("Content-Type", "application/json");
    int httpResponseCode = httpClient.POST(jsonString);

    if (httpResponseCode > 0) {
        Serial.println("Node health sent successfully. HTTP Response code: " + String(httpResponseCode));
    } else {
        Serial.println("Error sending node health. HTTP Response code: " + String(httpResponseCode));
    }

    httpClient.end();
	
}

bool CommunicationManager::publishEvent(const String& eventType, const String& sensorType) {
	
}
