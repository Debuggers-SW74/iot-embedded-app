/*CONFIGURATION LAYER: Threshold Management*/

#include "ThresholdManagement.h"

ThresholdManagement::ThresholdManagement(StateManagement* stateManager)
  : stateManager(stateManager) {

}

bool ThresholdManagement::updateThresholds(const ThresholdState& thresholds) {
  return stateManager->updateThresholds(thresholds);
}

bool ThresholdManagement::updateTemperatureThreshold(float max) {
  ThresholdState thresholds = stateManager->getThresholds();
  thresholds.temperatureMax = max;
  return stateManager->updateThresholds(thresholds);
}

bool ThresholdManagement::updateHumidityThreshold(float max) {
  ThresholdState thresholds = stateManager->getThresholds();
  thresholds.humidityMax = max;
  return stateManager->updateThresholds(thresholds);
}

bool ThresholdManagement::updatePressureThreshold(float max) {
  ThresholdState thresholds = stateManager->getThresholds();
  thresholds.pressureMax = max;
  return stateManager->updateThresholds(thresholds);
}

const ThresholdState& ThresholdManagement::getThresholds() const {
  return stateManager->getThresholds();
}
