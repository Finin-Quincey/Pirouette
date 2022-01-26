#include "Digit.h"

Digit::Digit(uint8_t upperPin1, uint8_t upperPin2, uint8_t upperPin3, uint8_t upperPin4, float upperOffset,
             uint8_t lowerPin1, uint8_t lowerPin2, uint8_t lowerPin3, uint8_t lowerPin4, float lowerOffset,
             uint8_t switchPin):

    // Initialise const members
    switchPin(switchPin){

    // Initialise stepper objects
    upperStepper = AccelStepper(AccelStepper::FULL4WIRE, upperPin1, upperPin2, upperPin3, upperPin4);
    lowerStepper = AccelStepper(AccelStepper::FULL4WIRE, lowerPin1, lowerPin2, lowerPin3, lowerPin4);

    attachInterrupt(digitalPinToInterrupt(switchPin), this->onLimitSwitchPressed, FALLING);

    
}