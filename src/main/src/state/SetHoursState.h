// ======================================================================================================
// SetHoursState.h
// ======================================================================================================
// Class representing the state of the clock when setting hours.
// ======================================================================================================

#ifndef SetHoursState_h
#define SetHoursState_h

#include "State.h"
#include "../Controller.h"
#include "../Utils.h"
#include "../component/LED.h"

class SetHoursState : public State {

    public:

        SetHoursState();

        void onEntry();

        State* update();

};

#endif