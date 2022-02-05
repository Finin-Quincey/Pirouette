// External libraries
#include <Arduino.h>

// Internal classes
#include "AccelStepper.h"

AccelStepper stepper(AccelStepper::FULL4WIRE, 22, 24, 23, 25);

int speed = 20;

void setup(){
    stepper.setMaxSpeed(2000); // WHY IS THE MAX SPEED 1 BY DEFAULT? WHO IS RUNNING A STEPPER AT 1 STEP PER SECOND?!
    stepper.setAcceleration(800);
}

void loop(){
    unsigned long t = millis();
    stepper.setSpeed(speed);
    while(millis() - t < 2000){
        stepper.runSpeed();
    }
    speed += 20;
}