#include "Digit.h"

// Init non-const static fields
int Digit::nextIndex = 0;
Digit *Digit::digits[] = {};

Digit::Digit(uint8_t upperPin1, uint8_t upperPin2, uint8_t upperPin3, uint8_t upperPin4, float upperOffset,
             uint8_t lowerPin1, uint8_t lowerPin2, uint8_t lowerPin3, uint8_t lowerPin4, float lowerOffset,
             uint8_t switchPin):
            // Initialise const members
            switchPin(switchPin),
            displayedNumeral(0){

    if(nextIndex > DIGIT_ARR_CAPACITY){
        error("Too many digits!");
        return;
    }

    digits[nextIndex++] = this; // Add to static array

    // Initialise stepper objects
    upperStepper = AccelStepper(AccelStepper::FULL4WIRE, upperPin1, upperPin2, upperPin3, upperPin4);
    lowerStepper = AccelStepper(AccelStepper::FULL4WIRE, lowerPin1, lowerPin2, lowerPin3, lowerPin4);
    
}

void Digit::zero(){
    
}

void Digit::update(){
    // Update stepper motors
    bool keepingUp = true;
    keepingUp &= upperStepper.run();
    keepingUp &= lowerStepper.run();
    // If either motor is falling behind, display an error
    if(!keepingUp) error("Can't keep up with the required stepping rate!");
}

void Digit::onLimitSwitchPressed(){

    // If only one motor is rotating, it's safe to assume it pressed the limit switch

    // If both motors are rotating but one is not near the limit switch, we assume it was the other one
    // The safety margin means this assumption is still valid for small amounts of drift

    // 

}

// Setters

void Digit::setDisplayedNumeral(uint8_t numeral){
    if(this->displayedNumeral == numeral) return; // Ignore if we're already displaying the requested number
    this->displayedNumeral = numeral;
    // Do stuff
}

// Getters

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