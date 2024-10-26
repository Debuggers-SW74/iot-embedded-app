/*COMPONENT LAYER: Actuator Driver*/

#ifndef ACTUATOR_DRIVER_H
#define ACTUATOR_DRIVER_H

class ActuatorDriver {
private:
    
public:
  explicit ActuatorDriver();
  void performAction(const String& actionType, const String& actionData);
};

#endif