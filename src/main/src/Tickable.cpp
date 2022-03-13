#include "Tickable.h"

// Init static fields
int Tickable::nextIndex = 0;
Tickable *Tickable::tickables[] = {};

Tickable::Tickable(){
    if(nextIndex >= ARR_CAPACITY){
        error("Maximum number of tickable objects reached");
    }
    tickables[nextIndex++] = this; // Add to static array
}

void Tickable::updateAll(){
    // Only iterate over the filled slots of the array
    for(int i=0; i<nextIndex; i++) tickables[i]->update();
}