// ======================================================================================================
// pirouette.ino
// ======================================================================================================
// Main sketch file that operates the full prototype clock, responsible for overall logic, updating the
// various components, defining interrupts, etc.
// ======================================================================================================

// External libraries
#include <Arduino.h>
#include <RTClib.h>

// Internal classes
#include "Digit.h"
#include "LED.h"
#include "Button.h"
#include "Speaker.h"
#include "Utils.h"

// Constants

const float LED_BRIGHTNESS = 0.4;
const hsv LED_CLR_ERROR = {0, 1, LED_BRIGHTNESS}; // Red

// Components

LED led(8, 7, 6); // Avoid pins 9 and 10, these use the same timer (2) as tone() so will interfere with it

Button selectBtn(4);
Button plusBtn  (5);
Button minusBtn (9);

Speaker speaker(12);

RTC_DS1307 rtc;

//                   Upper Stepper          Lower Stepper          LS
//                   1   2   3   4   OFF    1   2   3   4   OFF
Digit hrsTensDigit  (22, 23, 24, 25,   5.0, 50, 51, 52, 53, 278.0,  3);
Digit hrsUnitsDigit (26, 27, 28, 29,   9.0, 46, 47, 48, 49, 277.0,  2);
Digit minsTensDigit (30, 31, 32, 33,  13.0, 42, 43, 44, 45, 279.0, 18);
Digit minsUnitsDigit(34, 35, 36, 37,  10.0, 38, 39, 40, 41, 281.0, 19);

// Variables

bool setMins = false;
uint8_t hrs = 0;
uint8_t mins = 0;

bool initDone = false;

void setup(){

    // Init the objects

    // DEBUG
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    digitalWrite(9, 1); // Blue
    digitalWrite(10, 1); // Green
    digitalWrite(11, 1); // Red

    Serial.begin(9600);

    // RTC setup
    if(!rtc.begin()){
        error("Could not connect to RTC module");
    }
    if(!rtc.isrunning()){
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    Digit::initInterrupts(); // Finish setup

    // Zero everything
    hrsTensDigit.zero();
    hrsUnitsDigit.zero();
    minsTensDigit.zero();
    minsUnitsDigit.zero();

    led.setColour({0, 0, LED_BRIGHTNESS}); // White
    led.startEffect(LED::Effect::FLASH, 1000);
}

void loop(){

    if(errored){
        led.setColour(LED_CLR_ERROR);
        return; // Do nothing else
    }

    // DEBUG
    // Upper = orange, lower = purple, both = teal, neither = white
    // if(minsUnitsDigit.upperDisc.posDirty){
    //     led.setColour(minsUnitsDigit.lowerDisc.posDirty ? _TEAL : _ORANGE);
    // }else if(minsUnitsDigit.lowerDisc.posDirty){
    //     led.setColour(_PURPLE);
    // }else{
    //     led.setColour(_WHITE);
    // }

    if(selectBtn.justPressed() || plusBtn.justPressed() || minusBtn.justPressed())  speaker.playTone(1800, 50); // Beep boop

    if(!initDone){

        if(!hrsTensDigit.isZeroing() && !hrsUnitsDigit.isZeroing() && !minsTensDigit.isZeroing() && !minsUnitsDigit.isZeroing()){
            setHours(0);
            setMinutes(0);
            led.setColour({0, 1, LED_BRIGHTNESS}); // Red
            led.startEffect(LED::Effect::RAINBOW, 6000);
            initDone = true;
        }

    }else{
        
        // if(selectBtn.justPressed()){
        //     setMins = !setMins;
        // }else if(plusBtn.justPressed()){
        //     if(setMins) mins = modulo(mins + 1, 60);
        //     else hrs = modulo(hrs + 1, 24);
        // }else if(minusBtn.justPressed()){
        //     if(setMins) mins = modulo(mins - 1, 60);
        //     else hrs = modulo(hrs - 1, 24);
        // }

        DateTime now = rtc.now();

        setHours(now.hour());
        setMinutes(now.minute());

    }

    // Update everything that needs updating
    // This should always be done last to give us the best chance of detecting events that happened between updates,
    // before internal variables get updated
    Tickable::updateAll();
}

void setHours(uint8_t hours){

    if(hours > 23) error("Invalid hours!");
    
    hrsTensDigit  .setDisplayedNumeral(hours / 10);
    hrsUnitsDigit .setDisplayedNumeral(hours % 10);
}

void setMinutes(uint8_t minutes){

    if(minutes > 59) error("Invalid minutes!");
    
    minsTensDigit  .setDisplayedNumeral(minutes / 10);
    minsUnitsDigit .setDisplayedNumeral(minutes % 10);
}