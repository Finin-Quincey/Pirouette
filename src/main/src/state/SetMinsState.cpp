#include "SetMinsState.h"

SetMinsState::SetMinsState():
    State({175, 1, LED_BRIGHTNESS}, LED::Effect::FLASH, 1000){
}

void SetMinsState::onEntry(){
    State::onEntry(); // Call super
}

State* SetMinsState::update(){

    if(selectBtn.getHoldTime() > SELECT_HOLD_TIME) return STATE_CLOCK; // Finished setting clock

    if(selectBtn.justPressed()) return STATE_SET_HOURS;

    // Bit of a weird way of doing it but it avoids duplicating code without having to declare a separate function
    int delta = plusBtn.justPressed() - minusBtn.justPressed(); // Will be either -1, 0 or 1

    if(delta != 0){
        DateTime now = rtc.now();
        uint8_t minutes = modulo(now.minute() + delta, 60);
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), minutes, now.second()));
        setMinutes(minutes, false);
    }

    return this;
}