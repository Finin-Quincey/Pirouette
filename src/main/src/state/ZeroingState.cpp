#include "ZeroingState.h"

ZeroingState::ZeroingState():
    State({0, 0, LED_BRIGHTNESS}, LED::Effect::FLASH, 1000){
}

void ZeroingState::onEntry(){
    State::onEntry(); // Call super
    // Zero everything
    hrsTensDigit.zero();
    hrsUnitsDigit.zero();
    minsTensDigit.zero();
    minsUnitsDigit.zero();
}

State* ZeroingState::update(){

    if(!hrsTensDigit.isZeroing() && !hrsUnitsDigit.isZeroing() && !minsTensDigit.isZeroing() && !minsUnitsDigit.isZeroing()){
        led.setColour(_ORANGE);
        return STATE_CLOCK; // Switch to clock state when done zeroing
    }

    return this; // No state change
}