#include "TestState.h"

TestState::TestState():
    State({0, 0, LED_BRIGHTNESS}, LED::Effect::RAINBOW, 5000){
}

void TestState::onEntry(){
    State::onEntry(); // Call super
}

State* TestState::update(){
    return this;
}