#include "LED.h"

LED::LED(uint8_t pinR, uint8_t pinG, uint8_t pinB):
        // Init fields
        pinR(pinR),
        pinG(pinG),
        pinB(pinB),
        colour({0, 0, 1}),
        effect(NONE){
    
    if(!digitalPinHasPWM(pinR) || !digitalPinHasPWM(pinG) || !digitalPinHasPWM(pinB))
        error("Tried to create an LED with non-PWM pins!");
        return;

    pinMode(pinR, OUTPUT);
    pinMode(pinG, OUTPUT);
    pinMode(pinB, OUTPUT);

}

void LED::off(){
    effectPeriod = 0;
    setColourInternal(_BLACK);
}

void LED::on(){
    setColourInternal(colour);
}

void LED::setColour(hsv colour){
    effectPeriod = 0;
    this->colour = colour;
    setColourInternal(colour);
}

void LED::setColourInternal(hsv colour){
    rgb c = toRGB(colour);
    // The LED we're using is common-anode so the numbers are inverted, i.e. 255 is off and 0 is full brightness
    // Round is required or sometimes zeros won't quite turn the LED off
    analogWrite(pinR, round(255 * (1 - c.r)));
    analogWrite(pinG, round(255 * (1 - c.g)));
    analogWrite(pinB, round(255 * (1 - c.b)));
}

void LED::fadeTo(hsv colour, float duration){
    stopEffect();
    fadeColour = colour;
    // TODO
}

void LED::fadeOut(float duration){
    fadeTo(_BLACK, duration);
}

void LED::startEffect(Effect effect, float period){
    if(effectPeriod == 0) lastEffectTime = millis(); // Need this line or all the calcs will be wrong
    this->effect = effect;
    effectPeriod = period;
}

void LED::stopEffect(){
    effect = NONE;
    effectPeriod = 0;
}

void LED::update(){

    long t = millis();

    if(effectPeriod > 0){

        if(t - lastEffectTime > effectPeriod) lastEffectTime += effectPeriod;
        float f = (t - lastEffectTime) / effectPeriod;

        switch(effect){

            case FLASH:
                if(t - lastEffectTime > effectPeriod/2) setColourInternal(colour);
                else setColourInternal(_BLACK);
                break;
            
            // Need to use a block if declaring local variables in a case statement because the C++ compiler is dumb
            case RAINBOW: {
                float h = colour.h + 360 * f;
                if(h > 360) h -= 360;
                setColourInternal({h, colour.s, colour.v});
                } break;

            case BREATHE: {
                float v = colour.v * (sin(2*PI*f) + 1)/2; // Scale by original brightness
                setColourInternal({colour.h, colour.s, v});
                } break;

            default:
                break;
        }
    }
}