#include "Speaker.h"

Speaker::Speaker(uint8_t pin):
        // Init fields
        pin(pin){
    
    pinMode(pin, OUTPUT);
}

void Speaker::playTone(unsigned int frequency, unsigned long millis){
    tone(pin, frequency, millis);
}

void Speaker::silence(){
    noTone(pin);
}

void Speaker::update(){
    // TODO
}