#ifndef MultiStepper_h
#define MultiStepper_h

#include <Arduino.h>

// =================================================== Helper Methods ====================================================

float clamp180(float angle);

int modulo(int num, int denom); // Yep, even modulo doesn't work how you expect in C++ (% actually does REMAINDER)

// Returns -1 if x is negative, +1 if x is positive, and 0 if x is zero
double sign(double x);

// ============================================= Stepper motor wrapper class =============================================

// This is my custom version of the Stepper class in the Arduino library, with the following additional features:
// - Ability to drive multiple motors at once, i.e. it does not take control flow the entire time the stepper is moving
//   (this is theoretically limited by the Arduino's clock speed, but the steppers don't go anywhere near that fast)
// - Ability to automatically turn all pins off when the stepper halts - this saves power and prevents overheating when
//   the Arduino is turned on for extended periods of time

// For the sake of simplicity (and because this is my first time writing classes in C++), I'm defining this in the same
// file and not separating implementation from declaration. Not great practice, but this is only a test.

const int STEPS_PER_REVOLUTION = 2048;
const int PIN_COUNT = 4;
constexpr float SWITCH_CLEARANCE_ANGLE = 20;

constexpr int STEP_TABLE[][PIN_COUNT] = {
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 1},
    {1, 0, 0, 1}
};

class MultiStepper {

    // enum StepMode {
    //     FULL_STEP_SINGLE_COIL,
    //     FULL_STEP_DUAL_COIL,
    //     HALF_STEP
    // };

    // TODO: Implement half-stepping?

    public:

        enum Direction {
            STOPPED = 0,
            CLOCKWISE = 1,
            ANTICLOCKWISE = -1
        };

        static Direction directionFromValue(double v){
            if(v == 0) return STOPPED;
            return clamp180(v) > 0 ? CLOCKWISE : ANTICLOCKWISE;
        }


    private:

        //bool offWhenHalted = true;
        bool zeroing = false; // Anything that gets changed in an interrupt needs to be marked volatile
        bool switchWasOff = true;
        Direction direction = STOPPED;
        int currentStepIndex = 0;
        int *pins;
        int switchPin;
        // Angles are in degrees
        float currentAngle = 0;
        float targetAngle = 0;
        float angleOffset;
        int releaseTimer = 0;

    
    public:
        
        MultiStepper();

        // Sets up the stepper motor with the given pins
        void init(int pin1, int pin2, int pin3, int pin4, int switchPin, float angleOffset);

        float getCurrentAngle();
        Direction getDirection();

        // Stops the stepper motor immediately and cancels whatever it was doing
        void stop();

        // Sets all of this stepper's pins to LOW (i.e. turns all the coils off)
        void release();

        // Moves the stepper shaft to the given (absolute) angle of rotation
        void moveTo(double angle);

        // Moves the stepper shaft by the given angle relative to its current position
        void moveBy(double angle);

        // Zeroes the stepper motor by rotating it until the limit switch is pressed
        // Calls to update() are still required
        void zero();

        // Called when the limit switch for this motor is pressed
        // void onSwitchPress();

        // Called from the main program loop to update the stepper motor
        void update();

}; // C++ oddity number 43,562: classes need a semicolon after the closing brace

// One of my bugbears from the Java world is overuse of interfaces for EVERYTHING, and C++ is practically built on that
// concept... YAGNI is a powerful principle, don't write code just because someone somewhere *might* need it one day.
// At least when it's baked into the language we can kind of ignore it most of the time.
// \endrant

# endif