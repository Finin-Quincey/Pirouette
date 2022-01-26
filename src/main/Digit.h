// ======================================================================================================
// Digit.h
// ======================================================================================================
// Class representing one digit on the clock face, consisting of 2 discs, each driven by a stepper motor.
// This class serves to abstract (most of) the details of angles, zeroing and other things away from the
// main sketch, allowing it to focus on what actually needs to be displayed.
// ======================================================================================================

// Pre-processor junk
#ifndef Digit_h
#define Digit_h

// External libraries
#include <Arduino.h>
#include <AccelStepper.h>

// Internal classes
#include "Utils.h"

// TODO: Probably move these constants inside the class

// Angle lookup tables (for digits 0-9 in order)
static const double UPPER_ANGLES[] = {  0,  97,  70,  61, 228, 283, 283,   0, 180, 283};
static const double LOWER_ANGLES[] = {  0, 180,  72, 270, 180, 270,   0, 180,  25, 180};

class Digit {

    private:

        AccelStepper upperStepper;
        AccelStepper lowerStepper;

        const uint8_t switchPin;

    public:

        // Constructs a new Digit with the given parameters
        Digit(uint8_t upperPin1, uint8_t upperPin2, uint8_t upperPin3, uint8_t upperPin4, float upperOffset,
              uint8_t lowerPin1, uint8_t lowerPin2, uint8_t lowerPin3, uint8_t lowerPin4, float lowerOffset,
              uint8_t switchPin);

    private:

        void onLimitSwitchPressed();

};


#endif