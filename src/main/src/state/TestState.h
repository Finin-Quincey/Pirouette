// ======================================================================================================
// TestState.h
// ======================================================================================================
// Class representing the state of the clock when running the test sequence.
// ======================================================================================================

#ifndef TestState_h
#define TestState_h

#include "State.h"
#include "../Controller.h"
#include "../Utils.h"
#include "../component/LED.h"

class TestState : public State {

    public:

        TestState();

        void onEntry();

        State* update();

};

#endif