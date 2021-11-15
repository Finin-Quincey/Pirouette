#include <Stepper.h>
#include <Arduino.h>

const int STEPS_PER_REVOLUTION = 2048;
const int MOTOR_RPM = 10;

Stepper upperStepper(STEPS_PER_REVOLUTION, 8, 10, 9, 11);
Stepper lowerStepper(STEPS_PER_REVOLUTION, 2, 4, 3, 5);

const int SWITCH_PIN = 12;

const int UPPER_STEPPER_OFFSET = 200;
const int LOWER_STEPPER_OFFSET = 700;

const double DEGREES_PER_STEP = STEPS_PER_REVOLUTION / 360.0;

const double UPPER_ANGLES[] = {  0,  97,  70,  61, 231, 283, 283,   0, 180, 283};
const double LOWER_ANGLES[] = {  0, 180,  72, 272, 180, 272,   0, 180,  25, 180};

int displayedNumeral = 0;

void setup(){

    // Pin configuration
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    upperStepper.setSpeed(MOTOR_RPM);
    lowerStepper.setSpeed(MOTOR_RPM);

    Serial.begin(9600); // Start the serial monitor

    // Move both motors off of the switch if they are currently on it
    while(!digitalRead(SWITCH_PIN)){
        upperStepper.step(10);
        lowerStepper.step(10);
    }
    // Zero both motors
    zero(upperStepper);
    zero(lowerStepper);
    // Apply offsets to display the numeral zero
    upperStepper.step(UPPER_STEPPER_OFFSET);
    lowerStepper.step(LOWER_STEPPER_OFFSET);
}

void loop(){

    delay(3000);

    int prevDisplayedNumeral = displayedNumeral;
    displayedNumeral = (displayedNumeral + 1) % 10;

    // Really crude test for the time being, just move by the difference between successive numerals, clamped at -180 to 180
    upperStepper.step(angleToSteps(int(UPPER_ANGLES[displayedNumeral] - UPPER_ANGLES[prevDisplayedNumeral] + 180) % 360 - 180));
    lowerStepper.step(angleToSteps(int(LOWER_ANGLES[displayedNumeral] - LOWER_ANGLES[prevDisplayedNumeral] + 180) % 360 - 180));
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
