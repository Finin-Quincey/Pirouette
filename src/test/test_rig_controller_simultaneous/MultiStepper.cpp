#include "MultiStepper.h"

// =================================================== Helper Methods ====================================================

float clamp180(float angle){
    // Calculation steps:
    // abs(angle) - 180                                     How far outside the -180 to 180 range the input angle is
    // ceil((abs(angle) - 180) / 360)                       Number of times to add/subtract 360 to get the angle within range
    // sign(angle) * 360 * ceil((abs(angle) - 180) / 360)   Number to subtract from the angle to get it within range
    return angle - sign(angle) * 360 * ceil((abs(angle) - 180) / 360);
    //return int(int(angle + 360) % 360 + 180) % 360 - 180; // Old version
}

int modulo(int num, int denom){
    // https://stackoverflow.com/questions/11720656/modulo-operation-with-negative-numbers
    return (num % denom + denom) % denom;
}

double sign(double x){
    return (x > 0) - (x < 0); // Neat trick off of SO
}

// enum StepMode {
//     FULL_STEP_SINGLE_COIL,
//     FULL_STEP_DUAL_COIL,
//     HALF_STEP
// };

// TODO: Implement half-stepping?
        
// Took me forever to find out what was up with this, when you google it almost all the results seem to say just
// copy the array - to state the obvious, I don't WANT to copy the array, I want to pass it by reference!
// https://stackoverflow.com/questions/5724171/passing-an-array-by-reference

MultiStepper::MultiStepper(){
    // Do nothing because Arduino
}

void MultiStepper::init(int pin1, int pin2, int pin3, int pin4, int switchPin, float angleOffset){

    // Because C++ is backward, array-type variables/members can't be assigned, and because this is Arduino we
    // have no std::array or std::vector, so the best I can do without extra libraries is to use a pointer
    // Also remember that if you assign a pointer to a local variable, the memory it held can (i.e. assume it will) be
    // overwritten once the function (or other scope) containing that local variable exits - hence we have to assign
    // this->pins directly to the array declaration, we cannot have a local variable as a middleman without manipulating
    // memory explicitly with malloc() and so on

    // Store pin numbers for later
    this->pins = new int[PIN_COUNT]{pin1, pin2, pin3, pin4};
    this->switchPin = switchPin;
    this->angleOffset = clamp180(angleOffset);
    
    // Set up Arduino pins
    for(int i=0; i<PIN_COUNT; i++) pinMode(this->pins[i], OUTPUT);
    pinMode(switchPin, INPUT_PULLUP);

    switchWasOff = digitalRead(switchPin); // Need to read this now or we might think we just triggered it

}

float MultiStepper::getCurrentAngle(){ return currentAngle; }
MultiStepper::Direction MultiStepper::getDirection(){ return direction; }

void MultiStepper::release(){
    releaseTimer++;
}

void MultiStepper::moveTo(double angle){
    targetAngle = clamp180(angle);
    if(abs(clamp180(targetAngle - currentAngle)) < 180.0/STEPS_PER_REVOLUTION) return; // Ignore if the angles are the same
    direction = directionFromValue(targetAngle - currentAngle);
}

void MultiStepper::moveBy(double angle){
    moveTo(currentAngle + angle);
}

void MultiStepper::stop(){
    targetAngle = currentAngle;
    direction = STOPPED;
    release();
}

void MultiStepper::zero(){
    targetAngle = INFINITY; // Make target angle inf so we never reach it, otherwise we might stop too soon
    direction = CLOCKWISE;
    zeroing = true;
}

// Called when the limit switch for this motor is pressed
// void onSwitchPress(){
//     if(zeroing){
//         currentAngle = 0;
//         zeroing = false;
//         direction = STOPPED;
//         release();
//     }
// }

void MultiStepper::update(){
    // This is how we can have multiple steppers going at once
    // Member access is an unnecessarily complicated affair in C++:
    // Type::staticMember
    // object.nonStaticMember
    // pointer->nonStaticMember OR (*pointer).nonStaticMember
    // Remember that the 'this' keyword is a pointer! Luckily we can omit it unless there is ambiguity.

    if(releaseTimer > 0){
        releaseTimer++;
        if(releaseTimer > 100){
            for(int i=0; i<PIN_COUNT; i++) digitalWrite(this->pins[i], 0);
            releaseTimer = 0;
        }
    }

    if(direction == STOPPED) return; // Do nothing if we're stopped

    currentStepIndex = modulo(currentStepIndex + direction, PIN_COUNT); // Increment/decrement step index, wrapping if necessary

    // Pointers don't work with foreach because C++ doesn't know how long the pointed-to array is
    // (remember a pointer only ever points to 1 memory location, which in this case is the start of the array)
    // The saving grace here is a foreach wouldn't have been much use anyway since we need the index number directly
    for(int i=0; i<PIN_COUNT; i++) digitalWrite(this->pins[i], STEP_TABLE[currentStepIndex][i]);

    //for(int i=0; i<PIN_COUNT; i++) Serial.println(STEP_TABLE[currentStepIndex][i]);

    // Remember direction is also a number so we can do maths with it :)
    currentAngle += float(direction) * 360.0/STEPS_PER_REVOLUTION;
    currentAngle = clamp180(currentAngle);

    // Check whether the angle difference is less than half a step
    if(abs(clamp180(targetAngle - currentAngle)) < 180.0/STEPS_PER_REVOLUTION){
        direction = STOPPED;
        release();
    }

    bool switchOff = digitalRead(switchPin);

    // Deal with zeroing (we're not using interrupts for now, it doesn't make much difference)
    if(zeroing && !switchOff && switchWasOff){
        currentAngle = -angleOffset; // No need to actually move to the zero position, we might not want to!
        zeroing = false;
        moveBy(-SWITCH_CLEARANCE_ANGLE); // Move back a bit to clear the switch
    }

    switchWasOff = switchOff;
}

// One of my bugbears from the Java world is overuse of interfaces for EVERYTHING, and C++ is practically built on that
// concept... YAGNI is a powerful principle, don't write code just because someone somewhere *might* need it one day.
// At least when it's baked into the language we can kind of ignore it most of the time.
// \endrant