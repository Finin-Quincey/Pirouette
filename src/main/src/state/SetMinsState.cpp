#include "SetMinsState.h"

SetMinsState::SetMinsState():
    State({175, 1, LED_BRIGHTNESS}, LED::Effect::FLASH, 1000){
}

void SetMinsState::onEntry(){
    State::onEntry(); // Call super
}

State* SetMinsState::update(){
    return this;
}