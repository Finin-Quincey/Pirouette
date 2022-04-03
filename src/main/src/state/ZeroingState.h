// ======================================================================================================
// ZeroingState.h
// ======================================================================================================
// Class representing the state of the clock when zeroing its dials.
// ======================================================================================================

#ifndef ZeroingState_h
#define ZeroingState_h

#include "State.h"
#include "../Controller.h"
#include "../Utils.h"
#include "../component/LED.h"

class ZeroingState : public State {

    public:

        ZeroingState();

        void onEntry();

        State* update();

};

#endif