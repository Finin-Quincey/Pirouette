// ======================================================================================================
// LED.h
// ======================================================================================================
// Class representing a common-anode RGB LED. This deals with converting colours to PWM duty cycles
// (analog writes), as well as supporting various lighting effects including hue swirling, flashing,
// 'breathing' and smooth fading between colours.
// ======================================================================================================

#ifndef LED_h
#define LED_h

// External libraries
#include <Arduino.h>

// Internal classes
#include "Utils.h"

class LED {

    public:

        enum Effect {
            NONE,       // Lights the LED with a single constant colour (in other words, no special effects)
            FLASH,      // Flashes the LED on and off
            RAINBOW,    // Cycles the LED's hue at its current brightness and saturation
            BREATHE     // Fades the LED's brightness up and down
        };

    private:

        const uint8_t pinR, pinG, pinB;
        hsv colour;
        Effect effect;
        float effectPeriod;
        long lastEffectTime;

    public:

        LED(uint8_t pinR, uint8_t pinG, uint8_t pinB);

        void update();

        // Turns the LED off without setting the colour (cancels any effects)
        void off();

        // Turns the LED on without setting the colour, meaning it will be whatever colour it was previously
        void on();

        // Sets the LED to the given constant colour (cancels any effects)
        void setColour(hsv colour);
        
        // Fades the LED to the given colour over the specified length of time (in ms)
        void fadeTo(hsv colour, float duration);

        // Fades the LED to black (off) over the specified length of time (in ms)
        void fadeOut(float duration);

        // Starts the given lighting effect with the given cycle period (in ms)
        void startEffect(Effect effect, float period);

        // Stops the current lighting effect, leaving the LED in whatever state it is currently in
        void stopEffect();

    private:

        void setColourInternal(hsv colour);

};

#endif