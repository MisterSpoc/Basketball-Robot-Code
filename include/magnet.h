#ifndef magnet_h
#define magnet_h
#include <Arduino.h>

namespace magnet {
    #define magnet_trigger 42

    void setup();
    void enableMagnet();
    void disableMagnet();
}

#endif