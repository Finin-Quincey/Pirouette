#include "MultiStepper.h"


// ====================================================== Constants ======================================================

const float MOTOR_RPM = 10;
//const int STEPS_PER_REVOLUTION = 2048;
//const int ULONG_MAX_VALUE = 4294967295; // So we can deal with overflows, since the Arduino will be running 24/7

MultiStepper upperStepper = MultiStepper();
MultiStepper lowerStepper = MultiStepper();

static MultiStepper* steppers[] = {
    &upperStepper,
    &lowerStepper
};

const double UPPER_ANGLES[] = {  0,  97,  70,  61, 231, 283, 283,   0, 180, 283};
const double LOWER_ANGLES[] = {  0, 180,  72, 270, 180, 270,   0, 180,  25, 180};

int displayedNumeral = 0;

// Time between update cycles, in microseconds
unsigned long UPDATE_PERIOD = long(1000000 / (MOTOR_RPM / 60 * STEPS_PER_REVOLUTION));

int startup_flag = 0;

int counter = 0;


// ===================================================== Main Sketch =====================================================

void setup(){

    Serial.begin(9600); // Start the serial monitor

    upperStepper.init(8, 10, 9, 11, 13, 107);
    lowerStepper.init(3, 5, 4, 6, 13, -169);

    // Move the steppers off the limit switch if either one is on it to begin with (otherwise we can't zero them)
    // TODO: Ugly code, replace with something else that abstracts the pins behind MultiStepper
    for(int i=0; i<2; i++){

        for(MultiStepper* s : steppers) s->moveBy(180);

        // Because each switch is only shared by two steppers and the switch is on for less than 180deg, we can safely
        // assume that if we rotate them both at once the switch will open at some point
        while(!digitalRead(13)){
            for(MultiStepper* s : steppers) s->update();
            delay(2);
        }
    }

    for(MultiStepper* s : steppers) s->stop();

    // Zero the first motor
    upperStepper.zero();
    
}

void loop(){

    unsigned long t = micros();

    update_logic();

    for(MultiStepper* s : steppers) s->update();

    unsigned long elapsed = micros() - t; // Should wrap automatically

    unsigned int delayTime = UPDATE_PERIOD - elapsed;
    unsigned int delayMicros = delayTime % 1000;
    unsigned int delayMillis = delayTime / 1000;

    // delayMicroseconds doesn't work beyond 16383 so we need to split it up
    //delayMicroseconds(delayMicros);
    //delay(delayMillis);

    delay(2);

}

void update_logic(){
    
    if(startup_flag == 0){
        
        if(upperStepper.getDirection() == MultiStepper::Direction::STOPPED){
            // Zero the second motor when the first one has finished zeroing
            lowerStepper.zero();
            startup_flag = 1;
            Serial.println("Upper stepper zeroed");
        }

    }else if(startup_flag == 1){
        if(lowerStepper.getDirection() == MultiStepper::Direction::STOPPED){
            startup_flag = 2;
            Serial.println("Lower stepper zeroed");
        }

    }else{
        // Change the displayed numeral every few seconds (debug)
        if(counter++ > 3000){
            counter = 0;
            if(Serial.available() > 0){
                displayedNumeral = Serial.parseInt();
            }else{
                displayedNumeral = (displayedNumeral + 1) % 10;
            }
            Serial.println(displayedNumeral);
            upperStepper.moveTo(UPPER_ANGLES[displayedNumeral]);
            lowerStepper.moveTo(LOWER_ANGLES[displayedNumeral]);
        }
    }
}