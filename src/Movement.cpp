#include "SimpleRSLK.h"
#include "Movement.h"


Movement::Movement() {
}

// private variables

uint32_t Movement::tick_reference;
uint32_t Movement::buffer [];


// public variables
bool Movement::turn_in_progress;
bool Movement::driving_set_distance;
uint8_t Movement::speedR;
uint8_t Movement::speedL;
bool pidbool = false;
bool revbool = false;

// private functions

uint16_t Movement::angleToTicks(int16_t angle) {
    return abs(angle << 1);
}

   

// public functions

void Movement::enable() {
    setupRSLK();
    setupEncoder(72, 12, 56, 13);
    enableMotor(BOTH_MOTORS);
    speedR = 25; //default speed
    speedL = 25;
    turn_in_progress = false;
    driving_set_distance = false;
    tick_reference = 0;
}

void Movement::checks() {
    buffer [0] = getEncoderLeftCnt();
    buffer [1] = getEncoderRightCnt();

    // matches wheel speeds to keep robot driving straight
    // may want to replace with PID control but this is probably good enough

    if(driving_set_distance) {

        PID();
        if(getEncoderRightCnt() > tick_reference && getEncoderLeftCnt() > tick_reference) {
            halt();
            driving_set_distance = false;
            turn_in_progress = false;
        }
    } else if (pidbool) {
        PID();
    }

    // stops turn when desired angle is reached
    if(turn_in_progress) {
        PID();
        if(getEncoderLeftCnt() > tick_reference) {
            halt();
            turn_in_progress = false;
        } 
    }
}

void Movement::halt() {
    setMotorSpeed(BOTH_MOTORS, 0);
    pidbool = false;
    revbool = false;

}

void Movement::forward(bool wantpid) {
    setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
    setMotorSpeed(RIGHT_MOTOR, speedR);
    setMotorSpeed(LEFT_MOTOR, speedL);
    resetRightEncoderCnt();
    resetLeftEncoderCnt();
    pidbool = wantpid;
}

void Movement::reverse(bool wantpid) {
    revbool = true;
    setMotorDirection(BOTH_MOTORS, MOTOR_DIR_BACKWARD);
    setMotorSpeed(RIGHT_MOTOR, speedR);
    setMotorSpeed(LEFT_MOTOR, speedL);
    resetRightEncoderCnt();
    resetLeftEncoderCnt();
    pidbool = wantpid;
}

void Movement::rotate(bool direction) {

    int motor1, motor2;

    if (!direction) {
        motor1 = LEFT_MOTOR;
        motor2 = RIGHT_MOTOR;
    } else {
        motor1 = RIGHT_MOTOR;
        motor2 = LEFT_MOTOR;
    }

    setMotorDirection(motor1, MOTOR_DIR_BACKWARD);
    setMotorDirection(motor2, MOTOR_DIR_FORWARD);
    setMotorSpeed(RIGHT_MOTOR, speedR);
    setMotorSpeed(LEFT_MOTOR, speedL);
    resetRightEncoderCnt();
    resetLeftEncoderCnt();
}

void Movement::turn(int16_t angle) {
    if(!turn_in_progress) {
        turn_in_progress = true;
        tick_reference = angleToTicks(angle);
        rotate(signbit(angle));
        resetRightEncoderCnt();
        resetLeftEncoderCnt();
    }
}

void Movement::driveDistance(uint32_t distance) {
    if(!driving_set_distance) {
        driving_set_distance = true;
        tick_reference = uint32_t(360.0*distance/(7.0*3.14159));
        forward();
    }
}

void Movement::driveDistanceRev(uint32_t distance) {
    if(!driving_set_distance) {
        driving_set_distance = true;
        tick_reference = uint32_t(360.0*distance/(7.0*3.14159));
        reverse();
    }
}

void Movement::PID(){
    float Kp = 4.5; //5.3; //set up the constants value
    float Ki = 2;
    float Kd = 0.0;
    int P;
    int I;
    int D;

    uint32_t encoder_R = getEncoderRightCnt();
    uint32_t encoder_L = getEncoderLeftCnt();

    uint32_t error = encoder_R - encoder_L;
    
    int lastError = 0;
    boolean onoff = false;

    const uint8_t max_speed_R = 40;
    const uint8_t max_speed_L = 40;
    const uint8_t base_speed_R = 18;
    const uint8_t base_speed_L = 18;
    const uint8_t min_speed_R = 10;
    const uint8_t min_speed_L = 10;
    int motor_speed_L = 0;

    P = error;
    I = I + error;
    D = error - lastError;
    lastError = error;
    int motor_speed = P*Kp + I*Ki + D*Kd;

    motor_speed_L = base_speed_L + motor_speed;
  

    if (motor_speed_L > max_speed_L) {
        motor_speed_L = max_speed_L;
    }

    if (motor_speed_L < 0) {
        motor_speed_L = min_speed_L;
    } 

    setMotorSpeed(LEFT_MOTOR,motor_speed_L);
    Serial.println(speedL);
    }

