#include "Disc.h"

Disc::Disc(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, float offset):

        // Init fields
        offset(clamp180(offset)),
        // N.B. Drive mode must be specified or it will do nothing!
        stepper(AccelStepper(AccelStepper::FULL4WIRE, pin1, pin3, pin2, pin4)){

    // Set speed and acceleration
    stepper.setMaxSpeed(MAX_SPEED);
    stepper.setAcceleration(ACCELERATION);
}

void Disc::zero(){
    stepper.setCurrentPosition(-offset / STEP_ANGLE);
}

void Disc::update(){

    stepper.run();

    if(isMoving()){
        if(pinsOff) pinsOff == false;
    }else{
        if(!pinsOff){
            stepper.disableOutputs();
            pinsOff == true;
        }
    }
    //if(!keepingUp) error("Can't keep up with the required stepping rate!");
}

// The following methods are reversed compared to the original MultiStepper implementation: moveTo calls moveBy
// rather than the other way round. This way, moveBy can allow the motor can rotate through arbitrarily large
// angles while moveTo still chooses the shortest path as desired.

void Disc::moveTo(float angle){
    // There are multiple ways of implementing this:
    // 1. Use AccelStepper::moveTo and clamp the stepper position each time
    // 2. Use AccelStepper::moveTo and keep track of how many 'laps' it is on in this class
    // 3. Use AccelStepper::move and calculate the required delta in steps
    // 4. Call Disc::moveBy directly and calculate the required delta in degrees
    // I've gone with option 4 because it's simpler to implement and reuses existing code
    // N.B. we're not using getCurrentAngle as this would perform an unnecessary extra clamp180 call
    moveBy(clamp180(angle - stepper.currentPosition() * STEP_ANGLE));
}

void Disc::moveBy(float angle){
    stepper.move(angle / STEP_ANGLE);
}

void Disc::stop(){
    stepper.stop();
}

float Disc::getCurrentAngle(){
    return clamp180(stepper.currentPosition() * STEP_ANGLE);
}

bool Disc::isMoving(){
    return stepper.isRunning();
}

bool Disc::isNearLimitSwitch(){
    float delta = clamp180(stepper.currentPosition() * STEP_ANGLE - offset); // Angle from LS start position
    return delta > -LS_SAFETY_MARGIN && delta < LS_ON_REGION + LS_SAFETY_MARGIN;
}

uint8_t Disc::getCurrentDirection(){
    return sign(stepper.distanceToGo()); // TODO: Check if this really is zero when not moving
}