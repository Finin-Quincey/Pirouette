#include "Button.h"

Button::Button(uint8_t pin):
        // Init fields
        pin(pin),
        holdStartTime(0), // Wanted to use nan here just in case millis() returns zero, but that didn't work :(
        cooldownStartTime(0){

    pinMode(pin, INPUT_PULLUP);
}

void Button::update(){
    
    changed = false;

    if(cooldownStartTime > 0){
        if(millis() - cooldownStartTime > COOLDOWN) cooldownStartTime = 0;

    }else{
        if(holdStartTime == 0){
            if(isPressed()){
                holdStartTime = millis(); // Start the hold timer if the button just got pressed
                changed = true;
            }
        }else{
            if(!isPressed()){
                holdStartTime = 0; // Reset the hold timer if the button just got released
                cooldownStartTime = millis(); // Start the cooldown timer
                changed = true;
            }
        }
    }
}

bool Button::isPressed(){
    return !digitalRead(pin); // Buttons are pulled up so this is inverted
}

bool Button::justPressed(){
    return changed && isPressed();
}

bool Button::justReleased(){
    return changed && !isPressed();
}

unsigned long Button::getHoldTime(){
    return holdStartTime > 0 ? millis() - holdStartTime : 0;
}