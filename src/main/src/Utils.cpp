#include "Utils.h"

bool errored = false;

float clamp180(float angle){
    // Calculation steps:
    // abs(angle) - 180                                     How far outside the -180 to 180 range the input angle is
    // ceil((abs(angle) - 180) / 360)                       Number of times to add/subtract 360 to get the angle within range
    // sign(angle) * 360 * ceil((abs(angle) - 180) / 360)   Number to subtract from the angle to get it within range
    return angle - sign(angle) * 360 * ceil((abs(angle) - 180) / 360);
}

float clamp360(float angle){
    // This is actually the same as angle mod 360 but % (and hence the modulo function) only works for integral types
    float result = clamp180(angle - 180) + 180;
    return result == 360 ? 0 : result;
}

int modulo(int num, int denom){
    // https://stackoverflow.com/questions/11720656/modulo-operation-with-negative-numbers
    return (num % denom + denom) % denom;
}

double sign(double x){
    return (x > 0) - (x < 0); // Neat trick off of SO
}

void error(String message){
    Serial.println("Error: " + message);
    errored = true;
}

hsv toHSV(rgb in){

    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;

    if(delta < 0.00001){
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }

    if(max > 0.0){ // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    }else{
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    
    if(in.r >= max)                           // > is bogus, just keeps compilor happy
        out.h = (in.g - in.b) / delta;        // between yellow & magenta
    else if( in.g >= max )
        out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if(out.h < 0.0) out.h += 360.0;

    return out;
}


rgb toRGB(hsv in){

    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0){       // < is bogus, just shuts up warnings
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

    switch(i){

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