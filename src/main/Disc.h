// ======================================================================================================
// Disc.h
// ======================================================================================================
// Represents a single disc on the clock face (i.e. half a digit). This is essentially a wrapper class
// for AccelStepper from the AccelStepper library that deals with converting between angles and steps. It
// also handles shortest-path logic for moving the stepper to a given angle and turning off when stopped.
// ======================================================================================================

// Pre-processor junk
#ifndef Disc_h
#define Disc_h

// External libraries
#include <AccelStepper.h>

// Internal classes
#include "Utils.h"

// Constants
static const int STEPS_PER_REVOLUTION = 2048;                   // Number of steps to rotate the output shaft 360 degrees
static const float STEP_ANGLE = 360.0 / STEPS_PER_REVOLUTION;   // Angle the output shaft rotates through each step
static const int MAX_SPEED = 380;                               // Max motor speed in steps/s
static const int ACCELERATION = 800;                            // Motor acceleration in steps/s^2
static const float LS_ON_REGION = 45;                           // Size of the region in which the limit switch is pressed
static const float LS_SAFETY_MARGIN = 10;                       // Safety margin for assuming the limit switch isn't pressed

class Disc {

    private:

        AccelStepper stepper;
        float offset;
        bool pinsOff; // Keeps track of whether the pins are turned off or not, so we don't keep issuing write commands

    public:

        // Creates a new disc with the given pins and offset angle
        Disc(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, float offset);

        // Zeroes this disc's position by setting the current angle as its zero point, with the offset that
        // was specified when the disc was created. This is called when it triggers the limit switch.
        void zero();

        // Polls the disc's stepper motor - see AccelStepper::run() for more details
        void update();

        // Starts moving this disc to the given (absolute) angle of rotation.
        // The input angle will be clamped to between -180 and 180 degrees.
        // This method will always choose the direction that results in the least rotation (shortest path).
        void moveTo(float angle);

        // Starts moving this disc by the given angle relative to its current orientation.
        // Note that, unlike moveTo, this does not perform shortest-path logic, it just rotates the stepper
        // through the given angle even if that means rotating more than 180 degrees.
        void moveBy(float angle);

        // Brings this disc to a stop as soon as possible
        void stop();

        // Returns the current angle of this disc, clamped to the -180 to 180 degree range
        float getCurrentAngle();

        // Returns true if the disc is currently rotating, false otherwise
        bool isMoving();

        // Returns true if the disc is currently at an angle near the limit switch region (used for active zeroing)
        bool isNearLimitSwitch();

        // Returns 1 if the disc is rotating clockwise, -1 for anticlockwise, or 0 if it is stationary
        uint8_t getCurrentDirection(); // Not really worth making an enum for this

};

#endif