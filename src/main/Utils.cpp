#include "Utils.h"

float clamp180(float angle){
    // Calculation steps:
    // abs(angle) - 180                                     How far outside the -180 to 180 range the input angle is
    // ceil((abs(angle) - 180) / 360)                       Number of times to add/subtract 360 to get the angle within range
    // sign(angle) * 360 * ceil((abs(angle) - 180) / 360)   Number to subtract from the angle to get it within range
    return angle - sign(angle) * 360 * ceil((abs(angle) - 180) / 360);
}

int modulo(int num, int denom){
    // https://stackoverflow.com/questions/11720656/modulo-operation-with-negative-numbers
    return (num % denom + denom) % denom;
}

double sign(double x){
    return (x > 0) - (x < 0); // Neat trick off of SO
}