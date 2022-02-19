// ======================================================================================================
// pirouette.ino
// ======================================================================================================
// Main sketch file that operates the full prototype clock, responsible for overall logic, updating the
// various components, defining interrupts, etc.
// ======================================================================================================

// External libraries
#include <Arduino.h>

// Internal classes
#include "Digit.h"
#include "LED.h"
#include "Button.h"
#include "Speaker.h"
#include "Utils.h"

const float LED_BRIGHTNESS = 0.4;

LED led(8, 7, 6); // Avoid pins 9 and 10, these use the same timer (2) as tone() so will interfere with it

Button selectBtn(4);
Button plusBtn  (5);
Button minusBtn (9);

Speaker speaker(12);

//                   Upper Stepper          Lower Stepper          LS
//                   1   2   3   4   OFF    1   2   3   4   OFF
Digit hrsTensDigit  (22, 23, 24, 25,   5.0, 50, 51, 52, 53, 278.0,  3);
Digit hrsUnitsDigit (26, 27, 28, 29,   9.0, 46, 47, 48, 49, 277.0,  2);
Digit minsTensDigit (30, 31, 32, 33,  13.0, 42, 43, 44, 45, 279.0, 18);
Digit minsUnitsDigit(34, 35, 36, 37,  10.0, 38, 39, 40, 41, 281.0, 19);

int hrsTens = 1;
int hrsUnits = 9;
int minsTens = 4;
int minsUnits = 5;

void setup(){

    // Init the objects

    // DEBUG
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    digitalWrite(9, 1); // Blue
    digitalWrite(10, 1); // Green
    digitalWrite(11, 1); // Red

    Digit::initInterrupts(); // Finish setup

    // Zero everything
    hrsTensDigit.zero();
    hrsUnitsDigit.zero();
    minsTensDigit.zero();
    minsUnitsDigit.zero();

    led.setColour({0, 1, LED_BRIGHTNESS}); // Red
    led.startEffect(LED::Effect::RAINBOW, 6000);
}

void loop(){

    // Button + LED test
    //if(selectBtn.justPressed()) led.setColour({180, 1, LED_BRIGHTNESS}); // Cyan
    //if(plusBtn.justPressed())   led.setColour({270, 1, LED_BRIGHTNESS}); // Magenta
    //if(minusBtn.justPressed())  led.setColour({ 30, 1, LED_BRIGHTNESS}); // Orange

    //if(selectBtn.getHoldTime() > 3000) led.startEffect(LED::Effect::FLASH, 1000);

    // DEBUG
    // Upper = orange, lower = purple, both = teal, neither = white
    // if(minsUnitsDigit.upperDisc.posDirty){
    //     led.setColour(minsUnitsDigit.lowerDisc.posDirty ? _TEAL : _ORANGE);
    // }else if(minsUnitsDigit.lowerDisc.posDirty){
    //     led.setColour(_PURPLE);
    // }else{
    //     led.setColour(_WHITE);
    // }

    // Button + speaker test
    if(selectBtn.justPressed()) speaker.playTone(800, 100); // Boop
    if(plusBtn.justPressed())   speaker.playTone(1000, 100); // Beep
    if(minusBtn.justPressed())  speaker.playTone(1200, 100); // Bip

    if(!hrsTensDigit.isZeroing() && !hrsUnitsDigit.isZeroing() && !minsTensDigit.isZeroing() && !minsUnitsDigit.isZeroing()){
        if(millis() % 10000 == 0){
            updateLogic();
        }
    }

    // Update everything that needs updating
    // This should always be done last to give us the best chance of detecting events that happened between updates,
    // before internal variables get updated
    Digit::updateAll();
    led.update();
    selectBtn.update();
    plusBtn.update();
    minusBtn.update();
}

void updateLogic(){
    
    hrsTensDigit  .setDisplayedNumeral(hrsTens);
    hrsUnitsDigit .setDisplayedNumeral(hrsUnits);
    minsTensDigit .setDisplayedNumeral(minsTens);
    minsUnitsDigit.setDisplayedNumeral(minsUnits);

    minsUnits++;
    if(minsUnits > 9){
        minsUnits = 0;
        minsTens++;
        if(minsTens > 5){
            minsTens = 0;
            hrsUnits++;
            if(hrsTens == 2 && hrsUnits > 3){
                hrsUnits = 0;
                hrsTens = 0;
            }else if(hrsUnits > 9){
                hrsTens++;
                hrsUnits = 0;
            }
        }
    }
}