#ifndef Movement_h
#define Movement_h

#include "SimpleRSLK.h"
#define CCW false
#define CW true

class Movement
{
    private:
        static uint32_t tick_reference; // number of encoder ticks before wheel stops 
        static uint32_t buffer [2]; // holds motor tick counts to control speed;


        /// \brief converts desired rotation angle to encoder tick quantity
        /// \param[in] angle angle in degrees
        static uint16_t angleToTicks(int16_t angle);

    public:
        static bool turn_in_progress; // true if turn with desired angle is occurring
        static bool driving_set_distance;
        static uint8_t speedR; // percent of max speed 0 to 100
        static uint8_t speedL; // percent of max speed 0 to 100

        /// \brief empty Constructor
        /// 
        Movement();
        
        /// \brief Enables motors and sets defaults, MUST BE RUN IN SETUP BEFORE ANYTHING ELSE
        static void enable();

        /// \brief contains all logical checks for Movement functions. Must be run in void loop()
        static void checks();
        
        /// \brief Sets motor speed to 0
        static void halt();

        /// \brief Tells robot to move forward
        static void forward(bool wantpid = false);

        /// \brief Tells robot to move backward
        static void reverse(bool wantpid = false);

        /// \brief instructs robot to turn <angle> degrees
        /// \param[in] angle input angle in degrees. Can be positive or negative.
        static void turn(int16_t angle);

        /// \brief Rotates robot in place
        /// \param[in] direction "CW" or "CCW" for direction of rotation
        static void rotate(bool direction);

        /// \brief Has robot drive forwards set distance
        /// \param[in] distance distance in cm to drive forwards
        static void driveDistance(uint32_t distance);
        static void driveDistanceRev(uint32_t distance);

        static void PID();
};

#endif
