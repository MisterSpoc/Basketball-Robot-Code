#include "magnet.h"

void magnet::setup() {
    pinMode(magnet_trigger, OUTPUT);
}

void magnet::enableMagnet() {
    digitalWrite(magnet_trigger, HIGH);
}

void magnet::disableMagnet() {
    digitalWrite(magnet_trigger, LOW);
}