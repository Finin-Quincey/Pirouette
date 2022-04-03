#include "State.h"
#include "../Controller.h"

State::State(hsv ledColour, LED::Effect effect, float effectPeriod):
        ledColour(ledColour),
        effect(effect),
        effectPeriod(effectPeriod){
    // Nothing else here
}

void State::onEntry(){
    led.setColour(ledColour);
    led.startEffect(effect, effectPeriod);
}