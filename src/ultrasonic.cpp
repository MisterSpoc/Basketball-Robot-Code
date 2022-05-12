#include "ultrasonic.h"

void ultrasonic::setup() {
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}

float ultrasonic::getDistance() {
    unsigned long timer = 0;
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    timer = pulseIn(echo, HIGH);
    return 1e-4*(float(timer)*340.0)/2.0; // distance in cm

}