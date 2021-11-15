#include <Stepper.h>
#include <Arduino.h>

const int STEPS_PER_REVOLUTION = 2048;

Stepper upperStepper(STEPS_PER_REVOLUTION, 8, 9, 10, 11);
Stepper lowerStepper(STEPS_PER_REVOLUTION, 2, 3, 4, 5);

const int SWITCH_PIN = 12;

const int UPPER_STEPPER_OFFSET = 200;
const int LOWER_STEPPER_OFFSET = 700;

const double DEGREES_PER_STEP = STEPS_PER_REVOLUTION / 360.0;

void setup(){

    // Pin configuration
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    upperStepper.setSpeed(50);
    lowerStepper.setSpeed(50);

    Serial.begin(9600); // Start the serial monitor

    // Move both motors off of the switch if they are currently on it
    while(!digitalRead(SWITCH_PIN)){
        upperStepper.step(10);
        lowerStepper.step(10);
    }
    // Zero both motors
    zero(upperStepper);
    zero(lowerStepper);
    // Apply offsets to get to zero
    upperStepper.step(UPPER_STEPPER_OFFSET);
    lowerStepper.step(LOWER_STEPPER_OFFSET);
}

void loop(){

}

// Helper Methods

int angleToSteps(double angle){
    return (int)(round(angle * DEGREES_PER_STEP));
}

void zero(Stepper stepper){
    // Keep stepping the motor until it reaches the limit switch
    while(digitalRead(SWITCH_PIN)){
        stepper.step(1);
    }
    
    stepper.step(400); // Move the motor past the switch, otherwise we won't be able to zero the other one!
}
