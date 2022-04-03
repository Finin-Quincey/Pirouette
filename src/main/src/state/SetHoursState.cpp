#include "SetHoursState.h"

SetHoursState::SetHoursState():
    State({175, 1, LED_BRIGHTNESS}, LED::Effect::FLASH, 1000){
}

void SetHoursState::onEntry(){
    State::onEntry(); // Call super
}

State* SetHoursState::update(){

    if(selectBtn.getHoldTime() > SELECT_HOLD_TIME) return STATE_CLOCK; // Finished setting clock

    if(selectBtn.justPressed()) return STATE_SET_MINS;

    // Bit of a weird way of doing it but it avoids duplicating code without having to declare a separate function
    int delta = plusBtn.justPressed() - minusBtn.justPressed(); // Will be either -1, 0 or 1

    if(delta != 0){
        DateTime now = rtc.now();
        uint8_t hours = modulo(now.hour() + delta, 24);
        rtc.adjust(DateTime(now.year(), now.month(), now.day(), hours, now.minute(), now.second()));
        setHours(hours, false);
    }

    return this;
}