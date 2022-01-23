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
// - Ability to select between half-step and single-coil full-step and dual-coil full-step drive modes
// - Pins are in order rather than the 2nd and 3rd being swapped (why the heck is the Arduino one like this?!)

const int STEPS_PER_REVOLUTION = 2048;
const int PIN_COUNT = 4;
constexpr float SWITCH_CLEARANCE_ANGLE = 20;

// Full-step, single-coil drive ('wave drive')
// This is rarely used because it produces less torque, but since we don't need torque here it's actually quite helpful
// because it reduces* the likelihood of drift when the motor is powered off - for an explanation, see the following link:
// https://forum.arduino.cc/t/stopping-stepper-motor-drift-in-low-power-application/450790
// 
// + Lower power consumption
// + Higher top speed
// + Motor always stops on a single winding, making it unlikely to drift when powered off
// - Lower torque
// - Maybe more ringing? (since there's less magnetic force to hold it in place)
//
// ** I say 'reduces' because there's still a chance that the motor will drift for a variety of reasons, both mechanical and
// electrical. Testing may confirm how much chance remains.
constexpr int STEP_TABLE[][PIN_COUNT] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

// Full-step, dual-coil drive
//
// + Higher torque
// + Higher top speed
// - Higher power consumption
// - Motor always drifts half a step when powered off
// constexpr int STEP_TABLE[][PIN_COUNT] = {
//     {1, 1, 0, 0},
//     {0, 1, 1, 0},
//     {0, 0, 1, 1},
//     {1, 0, 0, 1}
// };

// Half-step drive
//
// + Finer position control (twice that of full-step, naturally)
// + Reasonably high torque
// - Lower top speed
// - 50% chance for the motor to drift half a step when powered off - can be mitigated by only powering off after an even
//   number of steps, but that kind of defeats the point!
// - Double the software performance overhead for the same speed compared to full-step drive modes
// constexpr int STEP_TABLE[][PIN_COUNT] = {
//     {1, 0, 0, 0},
//     {1, 1, 0, 0},
//     {0, 1, 0, 0},
//     {0, 1, 1, 0},
//     {0, 0, 1, 0},
//     {0, 0, 1, 1},
//     {0, 0, 0, 1},
//     {1, 0, 0, 1}
// };

class MultiStepper {

    public:

        enum Direction {
            STOPPED = 0,
            CLOCKWISE = 1,
            ANTICLOCKWISE = -1
        };

        enum StepMode {
            FULL_STEP_SINGLE_COIL,
            FULL_STEP_DUAL_COIL,
            HALF_STEP
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