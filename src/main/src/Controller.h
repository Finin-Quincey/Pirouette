// ======================================================================================================
// Controller.h
// ======================================================================================================
// Main file that operates the full prototype clock, responsible for overall logic, updating the various
// components, defining interrupts, etc.
// ======================================================================================================

#ifndef Controller_h
#define Controller_h

// External libraries
#include <Arduino.h>
#include <RTClib.h>

// Internal classes
#include "Utils.h"

#include "component/Digit.h"
#include "component/LED.h"
#include "component/Button.h"
#include "component/Speaker.h"

#include "state/State.h"
#include "state/ZeroingState.h"
#include "state/ClockState.h"
#include "state/SetHoursState.h"
#include "state/SetMinsState.h"
#include "state/TestState.h"

// Constants

const unsigned long SELECT_HOLD_TIME = 3000;
const float LED_BRIGHTNESS = 0.25;
const hsv LED_CLR_ERROR = {0, 1, LED_BRIGHTNESS}; // Red
const unsigned long LED_SLEEP_TIME = 10000;

// Components

extern LED led;

extern Button selectBtn;
extern Button plusBtn;
extern Button minusBtn;

extern Speaker speaker;

extern RTC_DS1307 rtc;

extern Digit hrsTensDigit;
extern Digit hrsUnitsDigit;
extern Digit minsTensDigit;
extern Digit minsUnitsDigit;

// States

// Static pointers to each state
// These are singletons because creating a new state each time we switched would be wasteful and unnecessary
// They're stored here rather than in their respective classes so states don't reference each other
extern State* STATE_ZEROING;
extern State* STATE_TEST;
extern State* STATE_CLOCK;
extern State* STATE_SET_HOURS;
extern State* STATE_SET_MINS;

void do_setup();

void do_loop();

// Sets the left-hand display to the given number (need not be a number of hours, it's just convenient to call it that)
void setHours(uint8_t hours, bool force);

// Sets the right-hand display to the given number (need not be a number of minutes, it's just convenient to call it that)
void setMinutes(uint8_t minutes, bool force);

#endif