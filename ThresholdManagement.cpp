/*CONFIGURATION LAYER: Threshold Management*/

#include "ThresholdManagement.h"

ThresholdManagement::ThresholdManagement(StateManagement* stateManager)
  : stateManager(stateManager) {

}

bool ThresholdManagement::updateThresholds(const ThresholdState& thresholds) {
  return stateManager->updateThresholds(thresholds);
}

bool ThresholdManagement::updateTemperatureThreshold(float max) {

}

bool ThresholdManagement::updateHumidityThreshold(float max) {
  ThresholdState thresholds = stateManager->getThresholds();
  thresholds.humidityMax = max;
  return stateManager->updateThresholds(thresholds);
}

bool ThresholdManagement::updatePressureThreshold(float max) {

}

const ThresholdState& ThresholdManagement::getThresholds() const {
  return stateManager->getThresholds();
}
