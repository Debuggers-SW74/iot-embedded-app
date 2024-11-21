#include "ActuatorDriver.h"

ActuatorDriver::ActuatorDriver()
    : greenLedPin(GREEN_LED_PIN), redLedPin(RED_LED_PIN), buzzerPin(BUZZER_PIN) {}

void ActuatorDriver::init() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  
  digitalWrite(greenLedPin, HIGH); // Initialize green LED status as on
  digitalWrite(redLedPin, LOW);
  noTone(buzzerPin);               // the buzzer must be off at start
}

void ActuatorDriver::turnOnGreenLed() {
  digitalWrite(greenLedPin, HIGH);
}

void ActuatorDriver::turnOffGreenLed() {
  digitalWrite(greenLedPin, LOW);
}

void ActuatorDriver::turnOnRedLed() {
  digitalWrite(redLedPin, HIGH);
}

void ActuatorDriver::turnOffRedLed() {
  digitalWrite(redLedPin, LOW);
}

void ActuatorDriver::activateBuzzer() {
  tone(buzzerPin, 1000); // Activate the buzzer with a 1000 Hz tone
}

void ActuatorDriver::deactivateBuzzer() {
  noTone(buzzerPin);    // Deactivate the buzzer
}