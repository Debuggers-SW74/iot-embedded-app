/*CONFIGURATION LAYER: Threshold Management*/

#include "ThresholdManagement.h"

ThresholdManagement::ThresholdManagement(StateManagement* stateManager)
  : stateManager(stateManager) {

}

bool ThresholdManagement::updateThresholds(const ThresholdState& thresholds) {

}

bool ThresholdManagement::updateTemperatureThreshold(float max) {

}

bool ThresholdManagement::updateHumidityThreshold(float max) {

}

bool ThresholdManagement::updatePressureThreshold(float max) {

}

const ThresholdState& ThresholdManagement::getThresholds() const {

}