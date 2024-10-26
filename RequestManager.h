/*INTERFACE LAYER: Request Manager*/

#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include "StateManagement.h"

class RequestManager {
private:
  StateManagement* stateManager;
public:
  explicit RequestManager(StateManagement* stateManager);
    
  SensorState getState() const;                                // GET    State
  bool updateState(const SensorState& newState);               // UPDATE State
  bool updateThresholds(const ThresholdState& newThresholds);  // UPDATE Threshold
  HealthStatus getHealth() const;                              // GET    Health
};

#endif
