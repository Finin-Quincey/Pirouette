// ======================================================================================================
// pirouette.ino
// ======================================================================================================
// Main sketch file that operates the full prototype clock, responsible for overall logic, updating the
// various components, defining interrupts, etc.
// ======================================================================================================

// External libraries
#include <Arduino.h>

// Internal classes
#include "Digit.h"

//                           Upper Stepper        Lower Stepper        LS
//                           1   2   3   4   OFF  1   2   3   4   OFF
Digit hrsTensDigit   = Digit(22, 23, 24, 25,   9, 26, 27, 28, 29,  12,  3);
Digit hrsUnitsDigit  = Digit(30, 31, 32, 33,  13, 34, 35, 36, 37,  11,  2);
Digit minsTensDigit  = Digit(50, 51, 52, 53, 279, 46, 47, 48, 49, 279, 18);
Digit minsUnitsDigit = Digit(42, 43, 44, 45, 280, 38, 39, 40, 41, 281, 19);


void setup(){

    // Init the objects

    Digit::initInterrupts(); // Finish setup

}

void loop(){

    // Update everything that needs updating

    Digit::updateAll();
}