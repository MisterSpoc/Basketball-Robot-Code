#ifndef Sensing_h
#define Sensing_h

#include "SimpleRSLK.h"
#define IR0 46 //P6_2
#define IR1 70 //P10_1
#define IR2 63 //P6_3
#define interruptPin 26 //P4_4

class Sensing
{

  private:
    // initialize bumper pins
    const static int BUMPER_SENSORS = 6; // number of bumper buttons
    const uint8_t BUMPER_PINS[BUMPER_SENSORS] = {BP_SW_PIN_0, BP_SW_PIN_1, BP_SW_PIN_2, BP_SW_PIN_3, BP_SW_PIN_4, BP_SW_PIN_5};
   

    volatile static byte state;

  public:
    static bool bumper_sensors[BUMPER_SENSORS]; // 0,1 = right, 2,3 = middle, 4,5 = left 

    /// \brief empty Constructor
    ///
    Sensing();
    // make initialization function for bumper_pins
    // define variables and make them private (put them above sensing)

    /// \brief Initializes IR sensors and bumper switches
    static void initialize();

    /// \brief Checks to see if any bumper switches have been pressed
    void checkBumpers();

    /// \brief Checks to see if any of the IR beacons have been detected
    static void beaconDetected();

    /// \brief Goes through array of pins and reads their values
    static int checkIR();

    static bool something(int i);
};

#endif