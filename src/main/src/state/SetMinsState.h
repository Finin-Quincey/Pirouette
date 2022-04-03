// ======================================================================================================
// SetMinsState.h
// ======================================================================================================
// Class representing the state of the clock when setting minutes.
// ======================================================================================================

#ifndef SetMinsState_h
#define SetMinsState_h

#include "State.h"
#include "../Controller.h"
#include "../Utils.h"
#include "../component/LED.h"

class SetMinsState : public State {

    public:

        SetMinsState();

        void onEntry();

        State* update();

};

#endif