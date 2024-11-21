/* CONFIGURATION LAYER: Threshold Management */

#ifndef THRESHOLD_MANAGEMENT_H
#define THRESHOLD_MANAGEMENT_H

#include "StateManagement.h"

struct IndicatorThreshold {
  float warningLevel;
  float criticalLevel;
};

struct EnvironmentThreshold {
  struct {
    float min;
    float max;
    float warningDelta;
  } temperature;
    
  struct {
    float min;
    float max;
    float warningDelta;
  } humidity;
    
  struct {
    float min;
    float max;
    float warningDelta;
  } pressure;

  struct {
    float min;
    float max;
    float warningDelta;
  } gas;
};

class ThresholdManagement {
private:
  StateManagement* stateManager;
  IndicatorThreshold indicatorThresholds;
  EnvironmentThreshold environmentThresholds;

  bool validateTemperatureThreshold(float min, float max) const;
  bool validateHumidityThreshold(float min, float max) const;
  bool validatePressureThreshold(float min, float max) const;
  bool validateGasThreshold(float min, float max) const;
  ThresholdState convertToThresholdState() const;

public:
  explicit ThresholdManagement(StateManagement* stateManager);
  bool updateThresholds(const ThresholdState& thresholds);
  bool applyConfiguration();

  // Indicator Threshold
  bool setWarningLevel(float level);
  bool setCriticalLevel(float level);
  const IndicatorThreshold& getIndicatorThresholds() const;

  // Environment Threshold
  bool setTemperatureThresholds(float min, float max, float warningDelta);
  bool setHumidityThresholds(float min, float max, float warningDelta);
  bool setPressureThresholds(float min, float max, float warningDelta);
  bool setGasThresholds(float min, float max, float warningDelta);
  const EnvironmentThreshold& getEnvironmentThresholds() const;
};

#endif // THRESHOLD_MANAGEMENT_H