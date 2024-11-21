/* BEHAVIOR LAYER: Sensor Driver */

#include "SensorDriver.h"

/*BEM280 Sensor*/
SensorDriver::SensorDriver() : temperature(0.0), humidity(0.0), pressure(0.0), altitude(0.0) {
  Wire.begin();
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting I2C communication with the BME280...");
}

void SensorDriver::readSensorData() {
  Wire.beginTransmission(BME280_I2C_ADDRESS);
  Wire.write(0x00); // Command para leer la data
  Wire.endTransmission();
    
  Wire.requestFrom(BME280_I2C_ADDRESS, 4); 
  if (Wire.available() == 4) {
    temperature = Wire.read(); 
    humidity = Wire.read();    
    pressure = Wire.read();    
    altitude = Wire.read();
  } else { Serial.println("No se recibieron suficientes bytes."); }
}


void SensorDriver::collectData() {
  Serial.println("-------------------------------");
  Serial.println("COLLECT DATA BME280:");
  readSensorData();
  transformData();
    
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.println(" °C");
    
  Serial.print("Humedad: ");
  Serial.print(humidity);
  Serial.println(" %");
    
  Serial.print("Presión: ");
  Serial.print(pressure);
  Serial.println(" hPa");
    
  Serial.print("Altitud: ");
  Serial.print(altitude);
  Serial.println(" m");

  Serial.println("-------------------------------");
}

float SensorDriver::getTemperature() const { return temperature; }
float SensorDriver::getHumidity() const { return humidity; }
float SensorDriver::getPressure() const { return pressure; }
float SensorDriver::getAltitude() const { return altitude; }

void SensorDriver::transformData() { // Realizar transformación de datos (opcional)
  /*
    Por ejemplo, convertir los valores a otras unidades unidades deseadas:
    HectoPascales (hPa) -> Pascales (Pa)
    HectoPascales (hPa) -> Bares (Bar)
    Grados Celsius (°C) -> Grados Fahrenheit (°F)
  */
}


/*Gas Sensor MQ2*/
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
void SensorDriver::initGasSensor() {
  MQ2.setRegressionMethod(1);
  MQ2.setA(574.25);
  MQ2.setB(-2.222);
  MQ2.init();
  gasValue = 0.0;

  // Calibrar el sensor
  Serial.println("Calibrando sensor de gas, por favor espere.");
  float calcR0 = 0;
  for (int i =  1; i <= 10; i++) {
    MQ2.update();
    calcR0 = MQ2.calibrate(RatioMQ2CleanAir); // Ratio de aire limpio
    Serial.print(".");
  }
  MQ2.setR0(calcR0/10);
  Serial.println("\nCalibración completada.");

  // Exception Handling
  // Connection Issue Exception Handlers
  if(isinf(calcR0)) { Serial.println("Warning: R0 value is infinite. Please check your wiring."); while(1); }
  if(calcR0 == 0)   { Serial.println("Warning: R0 value is zero. Please check your wiring."); while(1); }

  // Show MQ2 Debug Log
  MQ2.serialDebug(true);

  // Setup Threshold Pin
  pinMode(Threshold, INPUT);
}

void SensorDriver::gasCollectData(){
  Serial.println("\nCOLLECT DATA MQ2 Gas Sensor:\n");
  MQ2.update();
  MQ2.readSensor();
  MQ2.serialDebug();

  // Read Value
  gasValue = analogRead(Pin);
  gasValue = map(gasValue, 0, 4095, 0, 100);

  Serial.print("Gas: ");
  Serial.print(gasValue);
  Serial.println(" %");
  Serial.println("-------------------------------");
}

float SensorDriver::getGasValue() const {
  return gasValue;
}