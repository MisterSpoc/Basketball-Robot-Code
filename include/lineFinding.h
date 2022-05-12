#ifndef lineFinding_h
#define lineFinding_h

#include "SimpleRSLK.h"

namespace lineSensing {

    void setup();
    void checks();
    bool atIntersection();
    bool onLine();
    void followLinePID();
    uint32_t linePos();

}

#endif
