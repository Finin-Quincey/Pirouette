#include <Stepper.h>
#include <Arduino.h>
#include <String.h>

// We're driving the motors in full-step mode so this number is half the total steps
const int stepsPerRevolution = 2048; // 64 half-steps per revolution, multiplied by 64x gear reduction

Stepper stepper(stepsPerRevolution, 8, 9, 10, 11);

const int SWITCH_PIN = 12;

int steps = 0;

void setup(){
    pinMode(SWITCH_PIN, INPUT_PULLUP);
    stepper.setSpeed(6); // 6 seems to be about the limit before it starts skipping steps
    Serial.begin(9600);
}

void loop(){
    stepper.step(1);
    steps++;
    if(!digitalRead(SWITCH_PIN)){ // If we've reached the limit switch
        Serial.println("Steps completed:"); // Print out the number of steps it took
        Serial.println(steps);
        delay(1000); // Wait 1 second
        stepper.step(400); // Move the motor past the switch (we know 400 is much less than a full revolution)
        steps = 400; // Reset the counter to 400 since we just did 400 steps
    }
}
