#include "stepper.h"
#include "magnet.h"

int Steps = 0;
int Direction = 1;
unsigned long rotation_count = 0;
unsigned long other_count = 0;
bool trigger_state = 0;
bool stepper::turn_in_progress = false;
bool stepper::arm_in_progress = false;

void stepper::setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(stepper_trigger, OUTPUT);
    pinMode(button, INPUT_PULLUP);
    digitalWrite(stepper_trigger, LOW);
}

void stepper::update() {
    if(turn_in_progress) {
        if(rotation_count > 0) {
            stepperFULL(1);
            rotation_count--;
        }
        else if(rotation_count == 0) {
            turn_in_progress = false;
        }
    }
    if(arm_in_progress) {
        if(digitalRead(button) == LOW) {
            rotation_count = 0;
            arm_in_progress = false;
            turn(-int(double(other_count)*360.0)/4096.0);
            other_count = 0;
            magnet::enableMagnet();
        }
        stepperFULL(1);
        other_count++;
    }
}

void stepper::arm() {
    arm_in_progress = true;
}


void stepper::turn(int angle) {
    if (angle < 0) {
        Direction = 0;
        angle = abs(angle);
    }
    else {
        Direction = 1;
    }
    double degrees_to_steps = 4096.0*double(angle)/360.0;
    rotation_count = long(degrees_to_steps);
    turn_in_progress = true;
}

void stepper::stepperFULL(int xw) {
    for (int x = 0; x < xw; x++) {
        switch (Steps) {
        case 0:
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            break;
        case 1:
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            break;
        case 2:
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            break;
        case 3:
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;
        case 4:
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;
        case 5:
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, HIGH);
            break;
        case 6:
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            break;
        case 7:
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            break;
        default:
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            break;
        }
        setDirectionFULL();
    }
}

void stepper::setDirectionFULL() {
    if (Direction == 1) {
        Steps++;
    }
    if (Direction == 0) {
        Steps--;
    }
    if (Steps > 7) {
        Steps = 0;
    }
    if (Steps < 0) {
        Steps = 7;
    }
}

void stepper::changeStepper() {
    trigger_state = !trigger_state;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(50);
    digitalWrite(stepper_trigger, trigger_state);
}