/*INTERFACE LAYER: Communication Manager*/

#include "CommunicationManager.h"
#include <ArduinoJson.h>

CommunicationManager::CommunicationManager(StateManagement* stateManager) 
    : stateManager(stateManager), connected(false) { }

bool CommunicationManager::connectWiFi() {

}

void CommunicationManager::sendState() {

}

String CommunicationManager::healthStatusToString(HealthStatus status) {

}

void CommunicationManager::sendHealth() {

}

bool CommunicationManager::publishEvent(const String& eventType, const String& sensorType) {
	
}