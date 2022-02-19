// ======================================================================================================
// Tickable.h
// ======================================================================================================
// Abstract base class for anything that requires continuous updates. This class defines a single
// instance method, update(), which subclasses implement to perform whatever logic they require.
// ======================================================================================================

#ifndef Tickable_h
#define Tickable_h

#include "Utils.h"

class Tickable {

    private:
        // Capacity of the internal tickables array
        static const int ARR_CAPACITY = 128;
        // Array of pointers to all instances of this class
        // Length is arbitrary but must be at least the number of instances we will have, obviously
        static Tickable *tickables[ARR_CAPACITY];
        // Next available index in the tickables array
        static int nextIndex;

    protected:

        Tickable();

        // Abstract update method for implementation by subclasses
        // Protected because we're not supposed to update things individually, that defeats the point
        virtual void update() = 0; // = 0 makes it 'pure', meaning it MUST be overridden by subclasses
        
        // N.B. Abstract =/= virtual! All abstract methods are virtual, but not all virtual methods are
        // abstract. Virtual basically means polymorphic, i.e. C++ IS NOT POLYMORPHIC BY DEFAULT!
        // Apparently polymorphism is slower, which I get, but why you would ever want the opposite
        // behaviour is beyond me, since it would mean an object behaves differently depending on the
        // type of your reference to it... oh well
        // This also explains why you can narrow method scope when overriding - as long as the method
        // isn't virtual
        // See https://stackoverflow.com/questions/2391679/why-do-we-need-virtual-functions-in-c

    public:

        // Updates all existing tickable objects
        static void updateAll();
};

# endif