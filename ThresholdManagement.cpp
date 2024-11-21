/* CONFIGURATION LAYER: Threshold Management */

#include "ThresholdManagement.h"

ThresholdManagement::ThresholdManagement(StateManagement* stateManager): stateManager(stateManager) {
  indicatorThresholds = {
    .warningLevel = 0.7f,
    .criticalLevel = 0.9f
  };

  environmentThresholds = {
    .temperature = {
      .min = 0.0f,
      .max = 85.0f,
      .warningDelta = 35.0f
    },
    .humidity = {
      .min = 0.0f,
      .max = 100.0f,
      .warningDelta = 50.0f
    },
    .pressure = {
      .min = 0.0f,
      .max = 255.0f,
      .warningDelta = 90.0f
    },
    .gas = {
      .min = 0.0f,
      .max = 100.0f,
      .warningDelta = 40.0f
    }
  };
}

bool ThresholdManagement::validateTemperatureThreshold(float min, float max) const {
  return (min >= 0.0f && max <= 85.0f && min < max);
}

bool ThresholdManagement::validateHumidityThreshold(float min, float max) const {
  return (min >= 0.0f && max <= 100.0f && min < max);
}

bool ThresholdManagement::validatePressureThreshold(float min, float max) const {
  return (min >= 3.0f && max <= 255.0f && min < max);
}

bool ThresholdManagement::validateGasThreshold(float min, float max) const {
  return (min >= 0.0f && max <= 100.0f && min < max);
}

ThresholdState ThresholdManagement::convertToThresholdState() const {
  return ThresholdState{
    .temperatureMax = environmentThresholds.temperature.max,
    .humidityMax = environmentThresholds.humidity.max,
    .pressureMax = environmentThresholds.pressure.max,
    .gasMax      = environmentThresholds.gas.max 
  };
}



/* Indicator Threshold */
/*--------------------*/
bool ThresholdManagement::setWarningLevel(float level) {
  if (level > 0.0f && level < indicatorThresholds.criticalLevel) {
    indicatorThresholds.warningLevel = level;
    return true;
  }
  return false;
}

bool ThresholdManagement::setCriticalLevel(float level) {
  if (level > indicatorThresholds.warningLevel && level <= 1.0f) {
    indicatorThresholds.criticalLevel = level;
    return true;
  }
  return false;
}

const IndicatorThreshold& ThresholdManagement::getIndicatorThresholds() const {
  return indicatorThresholds;
}



/* Environment Threshold */
/*--------------------*/
bool ThresholdManagement::setTemperatureThresholds(float min, float max, float warningDelta) {
  if (!validateTemperatureThreshold(min, max)) {
    Serial.println("Invalid temperature thresholds.");
    return false;
  }
    
  environmentThresholds.temperature = {min, max, warningDelta};
  Serial.println("Temperature thresholds set successfully.");
  return applyConfiguration();
}

bool ThresholdManagement::setHumidityThresholds(float min, float max, float warningDelta) {
  if (!validateHumidityThreshold(min, max)) { return false; }
  environmentThresholds.humidity = {min, max, warningDelta};
  return applyConfiguration();
}

bool ThresholdManagement::setPressureThresholds(float min, float max, float warningDelta) {
  if (!validatePressureThreshold(min, max)) { return false; }
  environmentThresholds.pressure = {min, max, warningDelta};
  return applyConfiguration();
}

bool ThresholdManagement::setGasThresholds(float min, float max, float warningDelta) {
  if (!validateGasThreshold(min, max)) { return false; }
  environmentThresholds.gas = {min, max, warningDelta};
  return applyConfiguration();
}

const EnvironmentThreshold& ThresholdManagement::getEnvironmentThresholds() const {
  return environmentThresholds;
}

bool ThresholdManagement::updateThresholds(const ThresholdState& thresholds) {


  return stateManager->updateThresholds(thresholds);
}

bool ThresholdManagement::applyConfiguration() {
  ThresholdState newThresholds = convertToThresholdState();
  return stateManager->updateThresholds(newThresholds);
}