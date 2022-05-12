#ifndef ultrasonic_h
#define ultrasonic_h

#include <Arduino.h>


namespace ultrasonic
{
    #define trig 59
    #define echo 23

    void setup();
    float getDistance();
}

#endif