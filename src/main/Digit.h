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

        // Bending over backwards to keep the callback definitions encapsulated in this class
        // C++ just makes it incredibly difficult to write good object-oriented code with proper
        // separation of concerns. I don't want the main sketch dealing with low-level ISRs!
        // There is a good reason for this beyond sheer stubbornness: it means I don't have to
        // repeat myself initialising each stepper's ISR manually (or manually make an array of them),
        // meaning no chance of forgetting if I were to somehow add another digit

        // Capacity of the internal instance array
        static const int DIGIT_ARR_CAPACITY = 10;
        // Array of pointers to all instances of this class
        // Length is arbitrary but must be at least the number of instances we will have, obviously
        static Digit *digits[DIGIT_ARR_CAPACITY];
        static int nextIndex;

    public:

        // Constructs a new Digit with the given parameters
        Digit(uint8_t upperPin1, uint8_t upperPin2, uint8_t upperPin3, uint8_t upperPin4, float upperOffset,
              uint8_t lowerPin1, uint8_t lowerPin2, uint8_t lowerPin3, uint8_t lowerPin4, float lowerOffset,
              uint8_t switchPin);

        // Sets up interrupt handlers (ISRs) for all digit instances according to their specified LS pin
        static void initInterrupts();

        // Updates all digit instances
        static void updateAll();

    private:

        // Updates this digit's logic and stepper motor controllers
        void update();

        // Called when a falling edge is detected on the limit switch for this digit
        void onLimitSwitchPressed();

};


#endif