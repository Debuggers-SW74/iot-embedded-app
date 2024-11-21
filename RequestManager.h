/* INTERFACE LAYER: Request Manager */

#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include "StateManagement.h"

class RequestManager {
private:
  StateManagement* stateManager;

public:
  explicit RequestManager(StateManagement* stateManager);
  SensorState getState() const;                               //[GET]: Get State
  bool updateState(const SensorState& newState);              //[PUT]: Update State
  bool updateThresholds(const ThresholdState& newThresholds); //[PUT]: Update Threshold
  HealthStatus getHealth() const;                             //[GET]: Get Health
};

#endif // REQUEST_MANAGER_H