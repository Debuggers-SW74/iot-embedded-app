# Fastporte IoT Sensor Project with ESP32

## Summary
The project shows how the IoT sensor could be able to reduce the risk of accidents in the transportation of hazardous materials and ensure that drivers do not suffer harm and companies do not lose money.. It integrates two custom chips: an MQ2 Gas Detector Module and a BME280 sensor, which reads temperature, pressure, altitude, and humidity. The application is developed using C++ Arduino Sketch.

## Hardware
- **ESP32**
- **MQ2 Gas Sensor Module**
- **BME280 Sensor Module**

## Libraries
- **WiFi**: For connecting the ESP32 to WiFi networks.
- **HttpClient**: To handle HTTP requests and responses.
- **ArduinoJson**: For parsing and generating JSON data.
- **MQUnifiedSensor**: For reading data from the MQ2 sensor.

## External Services
The application communicates with a RESTful API to send and receive sensor data. The API endpoint for this project is hosted at:
- **EDGE NODE API Endpoint**: [https://fastporte-edge-api.azurewebsites.net/swagger-ui/index.html#]

## Wokwi Custom Chips
This project utilizes Wokwi custom chips for simulating the MQ2 and BME280 sensors. You can find the public link to the Wokwi simulator here:
- [Wokwi Simulator Project](https://wokwi.com/projects/414946193954143233)

## Features
- **MQ2 Gas Detection**: Reads gas concentration levels and determines safety status.
- **BME280 Sensor**: Reads environmental data such as temperature, humidity, pressure, and altitude.
- **HTTP Communication**: Sends sensor data to a web API and receives updates for thresholds.

## Installation
1. Clone this repository to your local machine.
2. Open the project in the Arduino IDE.
3. Ensure you have the required libraries installed.
4. Configure the WiFi credentials and API endpoint in the code.
5. Upload the code to your ESP32 board.

## Usage
1. Power on the ESP32 and connect it to the WiFi network.
2. The application will start reading data from the MQ2 and BME280 sensors.
3. Sensor data will be sent to the configured API endpoint.
4. You can monitor the sensor readings and any threshold alerts via the API.
