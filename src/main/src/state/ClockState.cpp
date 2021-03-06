#include "ClockState.h"

ClockState::ClockState():
    State({175, 1, LED_BRIGHTNESS}, LED::Effect::NONE, 0){
}

void ClockState::onEntry(){
    State::onEntry(); // Call super
    this->displayCurrentTime(true);
}

State* ClockState::update(){

    if(selectBtn.getHoldTime() > SELECT_HOLD_TIME) return STATE_SET_HOURS; // Switch to set clock state

    // Primary state cycle
    // TODO: Potentially replace this with some internal mechanism
    // if(plusBtn.justReleased()) return STATE_TIMER;
    // if(minusBtn.justReleased()) return STATE_SCOREBOARD;

    this->displayCurrentTime(false);

    return this; // No state change
}

void ClockState::displayCurrentTime(bool force){

    // Get current time from RTC
    DateTime now = rtc.now();

    // Display time
    setHours(now.hour(), force);
    setMinutes(now.minute(), force);
}