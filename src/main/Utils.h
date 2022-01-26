// ======================================================================================================
// Utils.h
// ======================================================================================================
// Class containing various general helper functions, mathematical operations, and so on.
// ======================================================================================================

#ifndef Utils_h
#define Utils_h

// Shifts the given angle by a multiple of 360 such that -180 < angle <= 180
float clamp180(float angle);

// Returns the first argument MODULO the second argument, the way the % operator works in most languages
int modulo(int num, int denom); // Yep, even modulo doesn't work how you expect in C++ (% actually does REMAINDER)

// Returns -1 if x is negative, +1 if x is positive, and 0 if x is zero
double sign(double x);

// Prints the given error message to the serial monitor and turns the onboard LED red
void error(String message);

#endif