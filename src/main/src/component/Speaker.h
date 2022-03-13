// ======================================================================================================
// Speaker.h
// ======================================================================================================
// Class representing a speaker driven via the Arduino's tone function (either directly or more likely,
// via a MOSFET or other driver circuit).
// ======================================================================================================

#ifndef Speaker_h
#define Speaker_h

// External libraries
#include <Arduino.h>
#include "Tickable.h"

class Speaker : public Tickable {

    private:

        const uint8_t pin;

    public:

        Speaker(uint8_t pin);

        void playTone(unsigned int frequency, unsigned long millis);

        void silence();
        
        void update() override;
};

#endif