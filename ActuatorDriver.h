#ifndef ACTUATOR_DRIVER_H
#define ACTUATOR_DRIVER_H

#include <Arduino.h>
#define GREEN_LED_PIN 4 // Pin Green LED
#define RED_LED_PIN 2   // Pin Red LED
#define BUZZER_PIN 26   // Pin Buzzer

class ActuatorDriver {
private:
  int greenLedPin;
  int redLedPin;
  int buzzerPin;

public:
  explicit ActuatorDriver();
  void init();
  void turnOnGreenLed();
  void turnOffGreenLed();
  void turnOnRedLed();
  void turnOffRedLed();
  void activateBuzzer();
  void deactivateBuzzer();

  void playMelody();
  void startMelody();
  void stopMelody();
};

#endif // ACTUATOR_DRIVER_H