#include "Digit.h"

static int nextIndex = 0;

Digit::Digit(uint8_t upperPin1, uint8_t upperPin2, uint8_t upperPin3, uint8_t upperPin4, float upperOffset,
             uint8_t lowerPin1, uint8_t lowerPin2, uint8_t lowerPin3, uint8_t lowerPin4, float lowerOffset,
             uint8_t switchPin):

    // Initialise const members
    switchPin(switchPin){

    digits[nextIndex++] = this; // Add to static array

    // Initialise stepper objects
    upperStepper = AccelStepper(AccelStepper::FULL4WIRE, upperPin1, upperPin2, upperPin3, upperPin4);
    lowerStepper = AccelStepper(AccelStepper::FULL4WIRE, lowerPin1, lowerPin2, lowerPin3, lowerPin4);
    
}

void Digit::onLimitSwitchPressed(){

}

// Static methods

void Digit::initInterrupts(){
    // Can't have i as a local variable or the lambda would have to capture it, which attachInterrupt won't allow
    for(static int i=0; i<DIGIT_ARR_CAPACITY; i++){
        attachInterrupt(digitalPinToInterrupt(digits[i]->switchPin), []{digits[i]->onLimitSwitchPressed();}, FALLING);
    }
}

void Digit::updateAll(){
    for(Digit *d : digits) d->update();
}