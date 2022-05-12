#ifndef stepper_h
#define stepper_h
#include <Arduino.h>

class stepper 
{
    #define IN1 66
    #define IN2 67
    #define IN3 50
    #define IN4 62
    #define stepper_trigger 49
    #define button 38

    public:
        static bool turn_in_progress;
        static bool arm_in_progress;

        static void setup();
        static void update();
        static void turn(int angle);
        static void stepperFULL(int xw);
        static void setDirectionFULL();
        static void changeStepper();
        static void arm();
};

#endif