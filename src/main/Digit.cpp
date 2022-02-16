#include "Digit.h"

// Init non-const static fields
int Digit::numDigits = 0;
Digit *Digit::digits[] = {};

Digit::Digit(uint8_t upperPin1, uint8_t upperPin2, uint8_t upperPin3, uint8_t upperPin4, float upperOffset,
             uint8_t lowerPin1, uint8_t lowerPin2, uint8_t lowerPin3, uint8_t lowerPin4, float lowerOffset,
             uint8_t switchPin):

        // Initialise const members
        switchPin(switchPin),
        displayedNumeral(0),
        zeroingStep(0),
        upperDisc(Disc(upperPin1, upperPin2, upperPin3, upperPin4, upperOffset)),
        lowerDisc(Disc(lowerPin1, lowerPin2, lowerPin3, lowerPin4, lowerOffset)){

    if(numDigits > DIGIT_ARR_CAPACITY){
        error("Too many digits!");
        return;
    }

    digits[numDigits++] = this; // Add to static array

    // Configure limit switch pin
    pinMode(switchPin, INPUT_PULLUP);
}

void Digit::zero(){

    if(isZeroing()){
        error("Already zeroing!");
        return;
    }

    if(isLimitSwitchPressed()){
        // If the limit switch is pressed, we need to get the motors off the switch first
        // Because each switch is only shared by two steppers and the switch is on for less than 180deg, we can safely
        // assume that if we rotate them both at once the switch will open at some point
        upperDisc.moveBy(-360); // Rotating anticlockwise will reduce the overall time to zero later
        lowerDisc.moveBy(-360);
        zeroingStep = 1;

    }else{
        // If the limit switch is already open, we can skip straight to zeroing the upper motor
        // TODO: I don't like that we're repeating ourselves here, it would be nicer to have this in update() somehow
        upperDisc.moveBy(360);
        zeroingStep = 2;
    }
}

void Digit::update(){

    // Update stepper motors
    upperDisc.update();
    lowerDisc.update();

    // Update zeroing logic
    switch(zeroingStep){

        case 1: // Preparing to zero
            // Check if both motors are now off the switch and if so, stop them and start zeroing the upper motor
            // Don't need accuracy here so we're not using the interrupt (also it's a rising edge rather than falling)
            if(!isLimitSwitchPressed()){
                upperDisc.moveBy(360);
                // TODO: In theory there's a very small angle range in which the lower stepper could press the LS as it stops
                lowerDisc.stop();
                zeroingStep = 2;
            }
            break;

        case 2: // Zeroing upper motor
            if(!upperDisc.isMoving()){ // Done zeroing upper motor so move on to lower motor
                lowerDisc.moveBy(360);
                zeroingStep = 3;
            }
            break;

        case 3: // Zeroing lower motor
            if(!lowerDisc.isMoving()){
                zeroingStep = 0; // Done zeroing
            }
            break;
    }
}

void Digit::onLimitSwitchPressed(){

    //if(zeroingStep == 0) return; // DEBUG: Disable active zeroing

    // If only one motor is rotating, it's safe to assume it pressed the limit switch (we only ever zero during CW rotation)
    // If both motors are rotating but one is not near the limit switch, we assume it was the other one
    // The safety margin means this assumption is still valid for small amounts of drift
    if(upperDisc.getCurrentDirection() == 1 && (!lowerDisc.isMoving() || !lowerDisc.isNearLimitSwitch())){
        // TODO: At the moment we're modifying non-volatile variables in AccelStepper here, this may cause problems
        // However, if all we're going to do is set a flag then arguably that's no better than polling the switch pin
        // in update() anyway, although it would still have the advantage of debouncing the switch.
        // For now I'm going to take a chance and hope that the compiler won't optimise out anything important...
        upperDisc.zero(zeroingStep == 2);
        // If we're doing the zeroing sequence, immediately move the motor back a bit so it's not on the limit switch
        if(zeroingStep == 2) upperDisc.moveBy(-LS_SAFETY_MARGIN); // This should cancel the current move

    }else if(lowerDisc.getCurrentDirection() == 1 && (!upperDisc.isMoving() || !upperDisc.isNearLimitSwitch())){
        lowerDisc.zero(zeroingStep == 3);
        // If we're doing the zeroing sequence, stop the motor (no need to move it back since we're done zeroing)
        if(zeroingStep == 3) lowerDisc.stop();
    }
}

void Digit::setDisplayedNumeral(uint8_t numeral){

    if(numeral < 0 || numeral > 9) error("Invalid numeral!");
    
    if(this->displayedNumeral == numeral) return; // Ignore if we're already displaying the requested number
    this->displayedNumeral = numeral;
    
    upperDisc.moveTo(UPPER_ANGLES[numeral]);
    lowerDisc.moveTo(LOWER_ANGLES[numeral]);
}

bool Digit::isLimitSwitchPressed(){
    return !digitalRead(switchPin); // Remember the switch pins are inverted! (0/false means the switch is pressed)
}

bool Digit::isZeroing(){
    return zeroingStep != 0;
}

// Static methods

void Digit::initInterrupts(){
    // This is pretty horrible but it's the best I can do, at least I don't have to have separate static functions
    // or pass an object pointer into that object's own method
    // Can't use a loop because we'd either have to capture the index or make it static, meaning it will change before
    // the lambda uses it
    // At least this way, the number of interrupt pins is known so it kinda feels less horrible not to use a loop
    // In theory at least, we could have any number of digits and we could have more than one for the same interrupt pin
    attachInterrupt(digitalPinToInterrupt( 2), []{processInterrupt( 2);}, FALLING);
    attachInterrupt(digitalPinToInterrupt( 3), []{processInterrupt( 3);}, FALLING);
    attachInterrupt(digitalPinToInterrupt(18), []{processInterrupt(18);}, FALLING);
    attachInterrupt(digitalPinToInterrupt(19), []{processInterrupt(19);}, FALLING);
}

void Digit::processInterrupt(uint8_t pin){
    for(int i=0; i<numDigits; i++) if(digits[i]->switchPin == pin) digits[i]->onLimitSwitchPressed();
}

void Digit::updateAll(){
    //for(Digit *d : digits) d->update();
    for(int i=0; i<numDigits; i++) digits[i]->update();
}