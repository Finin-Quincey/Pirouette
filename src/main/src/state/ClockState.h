// ======================================================================================================
// ClockState.h
// ======================================================================================================
// Class representing the state of the clock when displaying the real-world time.
// ======================================================================================================

#ifndef ClockState_h
#define ClockState_h

#include <RTClib.h>

#include "State.h"
#include "../Controller.h"
#include "../Utils.h"
#include "../component/LED.h"

class ClockState : public State {

    public:

        ClockState();

        void onEntry();

        State* update();

    private:

        void displayCurrentTime(bool force);

};

#endif