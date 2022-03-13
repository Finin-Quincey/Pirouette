// ======================================================================================================
// Button.h
// ======================================================================================================
// Class representing a single momentary pushbutton. This class keeps track of button state, hold time
// and rising/falling edges.
// ======================================================================================================

#ifndef Button_h
#define Button_h

// External libraries
#include <Arduino.h>

// Internal classes
#include "Tickable.h"

static const unsigned long COOLDOWN = 200; // Minimum time between button presses in ms

class Button : public Tickable {
    
    private:

        const uint8_t pin;
        unsigned long holdStartTime; // Time at which the current press of the button started, in ms (zero if not pressed)
        unsigned long cooldownStartTime; // Time at which the most recent press of the button ended, in ms
        bool changed; // True if the state of the button changed during the last call to update(), false otherwise

    public:

        Button(uint8_t pin);

        // Updates this button's state/logic
        void update() override;

        // Returns true if this button is currently pressed, false otherwise
        // Note that this is true every tick while the button is held; if you need to detect rising or falling edges,
        // use justPressed() or justReleased(). Also note this does not respect button cooldown.
        bool isPressed();

        // Returns true if the button *just* got pressed (i.e. in the most recent update cycle), false otherwise
        bool justPressed();

        // Returns true if the button *just* got released (i.e. in the most recent update cycle), false otherwise
        bool justReleased();

        // Returns the length of time this button has been held for, in ms, or zero if not pressed
        unsigned long getHoldTime();

};

#endif