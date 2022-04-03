// ======================================================================================================
// State.h
// ======================================================================================================
// Abstract base class for all states of the clock program (state pattern).
// ======================================================================================================

#ifndef State_h
#define State_h

#include "../Utils.h"
#include "../component/LED.h"

class State {

    private:
    
        const hsv ledColour;
        const LED::Effect effect;
        const float effectPeriod;

    public:
    
        // Constructs a new state. Concrete subclasses should pass in a constant colour and LED effect.
        State(hsv ledColour, LED::Effect effect, float effectPeriod);
        // Returns the LED colour
        hsv getLEDColour();

        // Abstract methods

        // Called each time the clock switches to this state to perform any required initialisation
        virtual void onEntry();

        // Called each update cycle to perform any state-specific logic
        // Returns the new state to switch to (usually just this)
        virtual State* update() = 0;
};

#endif