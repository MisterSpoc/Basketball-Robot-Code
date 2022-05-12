#include "iic.h"


void IicSetup() {
    Wire.begin();
}

uint8_t getStatus() {
    Wire.requestFrom(Arduino, 1);
    uint8_t c = Wire.read();
    return c;
}

void send(char message []) {
    Wire.beginTransmission(Arduino);
    Wire.write(message);
    Wire.endTransmission();
    return;
}