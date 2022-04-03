#include "TestState.h"

TestState::TestState():
    State({0, 1, LED_BRIGHTNESS}, LED::Effect::RAINBOW, 5000){
}

void TestState::onEntry(){
    State::onEntry(); // Call super
}

State* TestState::update(){
    
    if(selectBtn.justReleased()) return STATE_ZEROING;

    hrsTensDigit  .upperDisc.moveBy(10);
    hrsTensDigit  .lowerDisc.moveBy(10);
    hrsUnitsDigit .upperDisc.moveBy(10);
    hrsUnitsDigit .lowerDisc.moveBy(10);
    minsTensDigit .upperDisc.moveBy(10);
    minsTensDigit .lowerDisc.moveBy(10);
    minsUnitsDigit.upperDisc.moveBy(10);
    minsUnitsDigit.lowerDisc.moveBy(10);

    return this;
}