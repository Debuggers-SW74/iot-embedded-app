/* INTERFACE LAYER: Communication Manager */

#include "CommunicationManager.h"
#include <ArduinoJson.h>

CommunicationManager::CommunicationManager(StateManagement* stateManager) 
  : stateManager(stateManager), connected(false) { }

String CommunicationManager::constructUrl(const char* endpoint) {
  return String(BASE_URL) + endpoint;
}

bool CommunicationManager::connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int retries = 20;
  while (WiFi.status() != WL_CONNECTED && retries > 0) {
    delay(500);
    Serial.print(".");
    retries--;
  }

  connected = (WiFi.status() == WL_CONNECTED);
    
  if (connected) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println("-------------------------------");
    
    if (getToken(MAC_ADDRESS)) { 
      Serial.println("\nToken obtained successfully"); 
      Serial.println("-------------------------------");

      if (getCurrentTripDetails()) {
        Serial.println("Current trip details obtained successfully.");
        Serial.println("-------------------------------");
      } else {
        Serial.println("Failed to obtain current trip details.");
        Serial.println("-------------------------------");
      }
    };
  } else {
    Serial.println("\nWiFi Connection Failed!");
  }

  return connected;
}


bool CommunicationManager::isConnected(){
  return connected;
}

String CommunicationManager::createJsonPayload(const SensorState& state) {
  StaticJsonDocument<200> doc;
  doc["temperature"] = state.temperature;
  doc["humidity"]    = state.humidity;
  doc["pressure"]    = state.pressure;
  doc["gas"]         = state.gas;
    
  String payload;
  serializeJson(doc, payload);
  Serial.println("Payload: " + payload + "\n");
  return payload;
}

String CommunicationManager::createHealthPayload(HealthStatus status) {
  StaticJsonDocument<64> doc;
  doc["status"] = (status == HealthStatus::HEALTHY) ? "HEALTHY" : "WARNING";

  String payload;
  serializeJson(doc, payload);
  return payload;
}

String CommunicationManager::createEventPayload(
    const String& eventType, 
    const String& sensorType,
    float currentValue,
    float thresholdValue
) {
    StaticJsonDocument<200> doc;
    doc["eventType"] = eventType;
    doc["sensorType"] = sensorType;
    doc["currentValue"] = currentValue;
    doc["thresholdValue"] = thresholdValue;
    doc["timestamp"] = millis();
    
    String payload;
    serializeJson(doc, payload);
    return payload;
}

bool CommunicationManager::sendRequest(const String& url, const String& method, const String& payload) {
  if (!isConnected() || token.isEmpty()) return false;

  Serial.println("HTTP Method: " + method);
  Serial.println("Sending payload: " + payload);
  Serial.println("To URL: " + url);

  httpClient.begin(url);
  httpClient.addHeader("Authorization", "Bearer " + token);
  httpClient.addHeader("Content-Type", "application/json");
    
  int httpResponseCode;
  if (method == "POST") { httpResponseCode = httpClient.POST(payload); } 
  else if (method == "PUT") { httpResponseCode = httpClient.PUT(payload); } 
  else { httpResponseCode = httpClient.GET(); }

  Serial.print("HTTP Response Code: ");
  Serial.println(httpResponseCode);

  // Verifica si la respuesta es un error
  if (httpResponseCode != 200 && httpResponseCode != 201 && httpResponseCode != 204) {
    String response = httpClient.getString();
    Serial.println("Error response: " + response);
  }

  //bool success = (httpResponseCode > 0);
  bool success = (httpResponseCode == 200 || httpResponseCode == 201 || httpResponseCode == 204);
  httpClient.end();
  return success;
}

bool CommunicationManager::sendState() {
  if (!isConnected()) {
    Serial.println("Not connected to WiFi. Cannot send state.");
    return false;
  }

  Serial.println("Preparing to send state...");
  const SensorState& state = stateManager->getSensorState();
  String payload = createJsonPayload(state);
  String url = constructUrl(STATE_ENDPOINT) + "?tripId=" + String(tripId);

  bool result = sendRequest(url, "POST", payload);
  if (!result) { Serial.println("Failed to send state."); }
  Serial.println("-------------------------------");
  return result;
}

bool CommunicationManager::sendHealth() {
  HealthStatus status = stateManager->getHealthStatus();
  String payload = createHealthPayload(status);
  return sendRequest(constructUrl(HEALTH_ENDPOINT), "POST", payload);
}

bool CommunicationManager::publishEvent(
    const String& eventType, 
    const String& sensorType,
    float currentValue,
    float thresholdValue
) {
  Serial.println("-------------------------------");
  String payload = createEventPayload(eventType, sensorType, currentValue, thresholdValue);
  return sendRequest(constructUrl(EVENT_ENDPOINT), "POST", payload);
}

bool CommunicationManager::getState() {
  if (!isConnected()) return false;

  String url = constructUrl(STATE_ENDPOINT);
  httpClient.begin(url);
    
  int httpResponseCode = httpClient.GET();

  if (httpResponseCode > 0) {
    String response = httpClient.getString();
    Serial.println("Response: " + response);
        
    //TODO: procesar la respuesta y actualizar el estado (opcional)
    //Suponemos que la respuesta es un JSON que contiene el estado del sensor
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, response);
    if (!error) {
      SensorState newState;
      newState.temperature = doc["temperature"];
      newState.humidity    = doc["humidity"];
      newState.pressure    = doc["pressure"];
      newState.gas         = doc["gas"];
      stateManager->updateSensorState(newState);

    } else { Serial.println("Failed to parse JSON response."); }
        
    httpClient.end();
    return true;

  } else {
    Serial.println("Error getting state. HTTP Response code: " + String(httpResponseCode));
    httpClient.end();
    return false;
  }
}

bool CommunicationManager::checkNewThresholds() {
  String url = constructUrl(THRESHOLD_ENDPOINT);
  httpClient.begin(url);
    
  int httpCode = httpClient.GET();
  bool hasUpdates = (httpCode == HTTP_CODE_OK);
    
  httpClient.end();
  return hasUpdates;
}

ThresholdState CommunicationManager::getNewThresholds() {
  String url = constructUrl(THRESHOLD_ENDPOINT) + "?tripId=" + String(tripId);
  httpClient.begin(url);
  httpClient.addHeader("Authorization", "Bearer " + token);
  httpClient.addHeader("Content-Type", "application/json");
    
  ThresholdState newThresholds{};
  int httpCode = httpClient.GET();
    
  if (httpCode == HTTP_CODE_OK) {
    String response = httpClient.getString();
    Serial.println("Threshold response: " + response);
    newThresholds = parseThresholdResponse(response);
  }
  else {
    Serial.println("Failed to get new thresholds. HTTP Response code: " + String(httpCode));
  }
    
  httpClient.end();
  return newThresholds;
}

ThresholdState CommunicationManager::parseThresholdResponse(const String& response) {
  //TODO: Lógica para parsear la respuesta y obtener los nuevos Thresholds

  ThresholdState newThresholds = {};

  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, response);
  
  if (!error) {
    newThresholds.temperatureMax = doc["temperatureMax"];
    newThresholds.humidityMax = doc["humidityMax"];
    newThresholds.pressureMax = doc["pressureMax"];
    newThresholds.gasMax = doc["gasMax"];
  } else {
    Serial.println("Failed to parse threshold response: " + String(error.c_str()));
  }
  
  return newThresholds;
}

bool CommunicationManager::getToken(const String& macAddress) {
  String url = "https://fastporte-edge-api.azurewebsites.net/api/v1/authentication/token/" + macAddress;
  httpClient.begin(url);
    
  int httpCode = httpClient.GET();

  if (httpCode == HTTP_CODE_OK) {
    String response = httpClient.getString();
    StaticJsonDocument<256> doc;
    deserializeJson(doc, response);
    token = doc["token"].as<String>(); // Extraer el token

    // Verifica que el token no esté vacío
    if (token.length() == 0) {
      Serial.println("Token is empty!");
      return false;
    }
        
    Serial.println("Token: " + token);
    return true;
  } else {
    Serial.println("Failed to obtain token");
    return false;
  }
}


/*Trip Data*/
/*----------------------*/
bool CommunicationManager::getCurrentTripDetails(){
  String url = constructUrl(CURRENT_TRIP) + "?driverId=" + String(driverId) + "&truckId=" + String(truckId);
  Serial.println("HTTP Method: GET");
  Serial.println("To URL: " + url);

  httpClient.begin(url);
  httpClient.addHeader("Authorization", "Bearer " + token);
  httpClient.addHeader("Content-Type", "application/json");

  int httpResponseCode = httpClient.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    String response = httpClient.getString();
    Serial.println("Current Trip Response: " + response);

    // Procesar la respuesta para extraer el tripId y otros detalles
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, response);
    if (!error) {
      tripId          = doc["tripId"];
      driverName      = doc["driverName"].as<String>();
      supervisorId    = doc["supervisorId"];
      supervisorName  = doc["supervisorName"].as<String>();

      Serial.println("Trip ID: " + String(tripId));
      Serial.println("Driver Name: " + driverName);
      Serial.println("Supervisor ID: " + String(supervisorId));
      Serial.println("Supervisor Name: " + supervisorName);

      httpClient.end();
      return true;
    } else {
      Serial.println("Failed to parse JSON response for trip details.");
      httpClient.end();
      return false;
    }
  } else {
    Serial.println("Error getting current trip details. HTTP Response code: " + String(httpResponseCode));
    httpClient.end();
    return false;
  }

  httpClient.end();
}

int CommunicationManager::getDriverId()          {return driverId;}
int CommunicationManager::getTruckId()           {return truckId;}
int CommunicationManager::getTripId()            {return tripId;}
String CommunicationManager::getDriverName()     {return driverName;}
int CommunicationManager::getSupervisorId()      {return supervisorId; }
String CommunicationManager::getSupervisorName() {return supervisorName;}



/*Send Real Time Data*/
/*----------------------*/
bool CommunicationManager::sendRealTimeData(){
  if (!isConnected()) {
    Serial.println("Not connected to WiFi. Cannot send real-time data.");
    return false;
  }

  Serial.println("-------------------------------");
  Serial.println("Sending real-time data...");
  String url = constructUrl(REAL_TIME_DATA);
    
  httpClient.begin(url);
  httpClient.addHeader("Authorization", "Bearer " + token);
  httpClient.addHeader("Content-Type", "application/json"); 

  int httpResponseCode = httpClient.POST("");
    
  Serial.print("HTTP Response Code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode == 200) {
    Serial.println("Readings sent successfully.");
    Serial.println("-------------------------------");
    return true;
  } else {
    Serial.println("Error sending readings. HTTP Response code: " + String(httpResponseCode));
    String response = httpClient.getString();
    Serial.println("Error response: " + response);

    Serial.println("-------------------------------");
    return false;
  }

  Serial.println("-------------------------------");
  httpClient.end();
}