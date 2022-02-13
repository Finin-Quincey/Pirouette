// ======================================================================================================
// Utils.h
// ======================================================================================================
// Class containing various general helper functions, mathematical operations, and so on.
// ======================================================================================================

#ifndef Utils_h
#define Utils_h

#include <Arduino.h>

// ======================================================================================================
// Mathematical functions
// ======================================================================================================

// Shifts the given angle by a multiple of 360 such that -180 < angle <= 180
float clamp180(float angle);

// Returns the first argument MODULO the second argument, the way the % operator works in most languages
int modulo(int num, int denom); // Yep, even modulo doesn't work how you expect in C++ (% actually does REMAINDER)

// Returns -1 if x is negative, +1 if x is positive, and 0 if x is zero
double sign(double x);

// ======================================================================================================
// Misc functions
// ======================================================================================================

// Prints the given error message to the serial monitor and turns the onboard LED red
void error(String message);

// ======================================================================================================
// Colour functions
// ======================================================================================================

// A colour represented in RGB colour space
typedef struct {
    double r;       // Red fraction between 0 and 1
    double g;       // Green fraction between 0 and 1
    double b;       // Blue fraction between 0 and 1
} rgb;

// A colour represented in HSV colour space
typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

hsv toHSV(rgb in);
rgb toRGB(hsv in);

const hsv _BLACK   = {  0, 0, 0};
const hsv _WHITE   = {  0, 0, 1};

const hsv _RED     = {  0, 1, 1};
const hsv _GREEN   = {120, 1, 1};
const hsv _BLUE    = {240, 0, 0};

const hsv _CYAN    = {180, 0, 0};
const hsv _MAGENTA = {300, 0, 0};
const hsv _YELLOW  = { 60, 0, 0};

const hsv _ORANGE  = { 30, 0, 0};
const hsv _LIME    = { 90, 0, 0};
const hsv _TEAL    = {150, 0, 0};
const hsv _L_BLUE  = {210, 0, 0};
const hsv _PURPLE  = {270, 0, 0};
const hsv _PINK    = {330, 0, 0};

#endif