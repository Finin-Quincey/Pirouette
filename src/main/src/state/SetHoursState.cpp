#include "SetHoursState.h"

SetHoursState::SetHoursState():
    State({175, 0, LED_BRIGHTNESS}, LED::Effect::FLASH, 1000){
}

void SetHoursState::onEntry(){
    State::onEntry(); // Call super
}

State* SetHoursState::update(){
    return this;
}