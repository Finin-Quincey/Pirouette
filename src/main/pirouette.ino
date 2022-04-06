// ======================================================================================================
// pirouette.ino
// ======================================================================================================
// Main sketch file. There is virtually nothing in here because .ino files can't be imported, meaning
// anything defined in here can only be accessed from files in the root sketch folder - so I've spared
// myself the pain and just moved everything into Controller.h.
// ======================================================================================================

#include "src/Controller.h"

void setup(){
    do_setup();
}

void loop(){
    do_loop();
}