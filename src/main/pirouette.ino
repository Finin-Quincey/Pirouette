#include "MultiStepper.h"


// ====================================================== Constants ======================================================

const float MOTOR_RPM = 12;

MultiStepper upperStepper1 = MultiStepper();
MultiStepper lowerStepper1 = MultiStepper();
MultiStepper upperStepper2 = MultiStepper();
MultiStepper lowerStepper2 = MultiStepper();
MultiStepper upperStepper3 = MultiStepper();
MultiStepper lowerStepper3 = MultiStepper();
MultiStepper upperStepper4 = MultiStepper();
MultiStepper lowerStepper4 = MultiStepper();

static MultiStepper* steppers[] = {
    &upperStepper1,
    &lowerStepper1,
    &upperStepper2,
    &lowerStepper2,
    &upperStepper3,
    &lowerStepper3,
    &upperStepper4,
    &lowerStepper4
};

const double UPPER_ANGLES[] = {  0,  97,  70,  61, 228, 283, 283,   0, 180, 283};
const double LOWER_ANGLES[] = {  0, 180,  72, 270, 180, 270,   0, 180,  25, 180};

int hrsTens = 1;
int hrsUnits = 1;
int minsTens = 1;
int minsUnits = 1;

// Time between update cycles, in microseconds
unsigned long UPDATE_PERIOD = long(1000000 / (MOTOR_RPM / 60.0 * STEPS_PER_REVOLUTION));

int startup_flag = 0;

int counter = 0;

int n = 0;

int PIN_R = 11;
int PIN_G = 10;
int PIN_B = 9;

int hue = 0;

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);


// ===================================================== Main Sketch =====================================================

void setup(){

    Serial.begin(9600); // Start the serial monitor

    upperStepper1.init(22, 23, 24, 25,  3, 9);
    upperStepper2.init(26, 27, 28, 29,  2, 12);
    upperStepper3.init(30, 31, 32, 33, 18, 13);
    upperStepper4.init(34, 35, 36, 37, 19, 11);
    lowerStepper1.init(50, 51, 52, 53,  3, 279);
    lowerStepper2.init(46, 47, 48, 49,  2, 279);
    lowerStepper3.init(42, 43, 44, 45, 18, 280);
    lowerStepper4.init(38, 39, 40, 41, 19, 281);

    pinMode(PIN_R, OUTPUT);
    pinMode(PIN_G, OUTPUT);
    pinMode(PIN_B, OUTPUT);

    // Remember these are inverted because the LED is common anode - 0, 0, 0 is white
    analogWrite(PIN_R, 0);
    analogWrite(PIN_G, 0);
    analogWrite(PIN_B, 0);

    // Zero everything
    upperStepper1.zero();
    upperStepper2.zero();
    upperStepper3.zero();
    upperStepper4.zero();

    for(int i=0; i<3000; i++){
        for(MultiStepper* s : steppers) s->update();
        delay(2);
    }

    lowerStepper1.zero();
    lowerStepper2.zero();
    lowerStepper3.zero();
    lowerStepper4.zero();

    for(int i=0; i<3000; i++){
        for(MultiStepper* s : steppers) s->update();
        delay(2);
    }
    
}

void loop(){
    
    // MultiStepper* s = steppers[n];

    // s->zero();

    // int i = 0;

    // // Twirly
    // while(s->getDirection() != MultiStepper::Direction::STOPPED && i < 3000){
    //     s->update();
    //     i++;
    //     delay(2);
    // }

    // //s->release();

    // n++;

    // if(n > 7) n = 0;

    unsigned long t = micros();

    update_logic();

    for(MultiStepper* s : steppers) s->update();

    hue = (hue + 1) % 1440;
    rgb clr = hsv2rgb({hue/4.0, 1, 0.5});

    analogWrite(PIN_R, 255 - clr.r * 255);
    analogWrite(PIN_G, 255 - clr.g * 255);
    analogWrite(PIN_B, 255 - clr.b * 255);

    unsigned long elapsed = micros() - t; // Should wrap automatically

    unsigned int delayTime = UPDATE_PERIOD - elapsed;
    unsigned int delayMicros = delayTime % 1000;
    unsigned int delayMillis = delayTime / 1000;

    // delayMicroseconds doesn't work beyond 16383 so we need to split it up
    delayMicroseconds(delayMicros);
    delay(delayMillis);

}

void update_logic(){
    // Change the displayed number every few seconds (debug)
    if(counter-- <= 0){

        counter = 3000;

        upperStepper1.moveTo(UPPER_ANGLES[hrsTens]);
        lowerStepper1.moveTo(LOWER_ANGLES[hrsTens]);
        upperStepper2.moveTo(UPPER_ANGLES[hrsUnits]);
        lowerStepper2.moveTo(LOWER_ANGLES[hrsUnits]);
        upperStepper3.moveTo(UPPER_ANGLES[minsTens]);
        lowerStepper3.moveTo(LOWER_ANGLES[minsTens]);
        upperStepper4.moveTo(UPPER_ANGLES[minsUnits]);
        lowerStepper4.moveTo(LOWER_ANGLES[minsUnits]);

        minsUnits++;
        if(minsUnits > 9){
            minsUnits = 0;
            minsTens++;
            if(minsTens > 5){
                minsTens = 0;
                hrsUnits++;
                if(hrsTens == 2 && hrsUnits > 3){
                    hrsUnits = 0;
                    hrsTens = 0;
                }else if(hrsUnits > 9){
                    hrsTens++;
                    hrsUnits = 0;
                }
            }
        }

    }
}

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}