#include "Controller.h"

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

// States

// Static pointers to each state
// These are singletons because creating a new state each time we switched would be wasteful and unnecessary
// They're stored here rather than in their respective classes so states don't reference each other
State* STATE_ZEROING   = new ZeroingState();
State* STATE_TEST      = new TestState();
State* STATE_CLOCK     = new ClockState();
State* STATE_SET_HOURS = new SetHoursState();
State* STATE_SET_MINS  = new SetMinsState();

// Side note: C++ has no less than FOUR different syntaxes for initialising something:
// 1. Foo foo = new Foo();      'Copy' initialisation (although apparently nothing is actually copied?)
// 2. Foo foo(new Foo());       Direct initialisation
// 3. Foo foo = {new Foo()};    'Copy' list initialisation
// 4. Foo foo{new Foo()};       Direct list initialisation
// For primitives, these are all equivalent other than the list functionality
// Apparently #4 always works, which is a pity because it's ugly as heck
// (Also, why is the best way to initialise a variable to make a list of length one?)

State* state = STATE_ZEROING;

uint8_t hrs = 0;
uint8_t mins = 0;

bool initDone = false;

void do_setup(){
    
    Serial.begin(9600);

    // RTC setup
    if(!rtc.begin()){
        error("Could not connect to RTC module");
    }
    if(!rtc.isrunning()){
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    Digit::initInterrupts(); // Finish setting up digit objects

    state->onEntry();

    // led.setColour({0, 0, LED_BRIGHTNESS}); // White
    // led.startEffect(LED::Effect::FLASH, 1000);
}

void do_loop(){

    if(errored){
        led.setColour(LED_CLR_ERROR);
        return; // Do nothing else
    }

    // Hold + and - for 3s to activate test sequence, regardless of current state
    if(plusBtn.getHoldTime() > SELECT_HOLD_TIME && minusBtn.getHoldTime() > SELECT_HOLD_TIME){
        state = STATE_TEST;
    }

    // TODO: Maybe make feedback state-specific? (i.e. don't beep if button does nothing in the current state)
    if(selectBtn.justPressed() || plusBtn.justPressed() || minusBtn.justPressed()) speaker.playTone(1800, 50); // Beep boop

    // Delegate to current state to perform state-specific logic and set return value as new state
    state = state->update();

    // Update everything that needs updating
    // This should always be done last to give us the best chance of detecting events that happened between updates,
    // before internal variables get updated
    Tickable::updateAll();
}

void setHours(uint8_t hours){

    if(hours > 99) error("Number cannot have more than 2 digits");
    
    hrsTensDigit  .setDisplayedNumeral(hours / 10);
    hrsUnitsDigit .setDisplayedNumeral(hours % 10);
}

void setMinutes(uint8_t minutes){

    if(minutes > 99) error("Number cannot have more than 2 digits");
    
    minsTensDigit  .setDisplayedNumeral(minutes / 10);
    minsUnitsDigit .setDisplayedNumeral(minutes % 10);
}