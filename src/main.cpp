#include <Arduino.h>
#include "Movement.h"
#include "Sensing.h"
#include "ultrasonic.h"
#include "stepper.h"
#include "magnet.h"
#include "lineFinding.h"

Movement movement;
Sensing sensing;
stepper step;



int count = 0;
bool oneTry = true;



int state = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  // module initializations
  movement.enable();
  sensing.initialize();
  ultrasonic::setup();
  step.setup();
  magnet::setup();
  lineSensing::setup();
}

void senseTest() { // prints IR sensor readings to the console
  int ir_result = sensing.checkIR();  
  Serial.print((ir_result>>2)%2);
  Serial.print(" ");
  Serial.print((ir_result>>1)%2);
  Serial.print(" ");
  Serial.print(ir_result%2);
  Serial.print(" ");
  Serial.println();
}

// main functions

bool dir = true; 

int16_t findBeacon(bool b) { // rotates robot past lit IR beacon. Use with movement.turn() to point directly at a beacon.
// NOTE: This code is blocking
  uint8_t IR_BUFFER_SIZE = 32;
  uint8_t i = 0;
  bool on = false;
  bool stop = false;
  uint8_t ir_buffer[IR_BUFFER_SIZE];
  uint32_t first_tick = 0;
  uint32_t last_tick = 0;

  for(int i = 0; i<IR_BUFFER_SIZE; i++)
    ir_buffer[i] = 1;

  movement.rotate("CCW");

  while(!stop) {
    ir_buffer[i] = (sensing.checkIR()>>1)%2; // bool on/off value of center IR sensor. NOTE: Signal is inverted, 1=off 0=on
    i++;
    
    if (i > IR_BUFFER_SIZE) {
      i = 0;
    }

    if(!on) {
      int avg = 0;
      for (int j=0; j<IR_BUFFER_SIZE; j++) {
        avg += ir_buffer[j];
      }
      if(double(avg)/double(IR_BUFFER_SIZE) <= 0.15) {
        on = true;
        first_tick = getEncoderLeftCnt();
      }
    }
    else {
      int avg = 0;
      for (int j=0; j<IR_BUFFER_SIZE; j++) {
        avg += ir_buffer[j];
      }
      if(double(avg)/double(IR_BUFFER_SIZE) >= 0.85) { //double(avg)/double(IR_BUFFER_SIZE) >= 0.9
        stop = true;
        last_tick = getEncoderLeftCnt();
        break;
      }
    }
    delay(5);
  }

  Serial.println("Halting");
  movement.halt();
  if(b) {
    state = 1;
  }
  delay(100);
  return ((last_tick - first_tick)/2 + 7);
}

void getToCenter() {
  
}

// test functions 
void moveTest() { // tests movement by making the robot rotate in place
    movement.checks();
  
  if(!movement.turn_in_progress) {
    switch(count) {
      case 0:
        delay(1000);
        movement.turn(80);
        count++;
        break;
      case 1:
        delay(1000);
        movement.turn(-180);
        count--;
        break;
    }
  }
}


void ultrasonicTest() { // prints distance to wall in mm to the console
  Serial.println(ultrasonic::getDistance());
}

double getAverage(int size, double *arr) {
  double avg = 0.0;
  for(int i=0; i<size; i++) {
    avg += arr[i];
  }
  return avg/size;
}

void drawArm(){
  step.turn(1430);
  magnet::enableMagnet();
}

void releaseArm(){
  int j = 0;
  if (j ==0){
    step.turn(-1405);
    j++;
  }
  else{ 
    magnet::disableMagnet();
  }
  
}

void feedBall(){
  step.turn(360);
}

// main loop

bool gettocenter = true;
int stepCounter = 0;
double distance1[7];
double distance2[10];
int i = 0;
bool test = true;
bool line = false;
const int buffersizes = 15;
double buffer1[buffersizes];
double buffer2[buffersizes];
float dist[10];
float avg_dist = 0.0;
bool intertest = false;

void loop() {
  // // run all update/check functions first, then enter into switch cases and other logic
  // update functions:
  movement.checks();
  Serial.println(state);

  switch (state) {
    case 0:
      movement.speedL = 18;
      movement.speedR = 18;
      movement.turn(findBeacon(true));
      break;
    case 1:
      if(!movement.turn_in_progress && !movement.driving_set_distance) {
        if(gettocenter){
          movement.speedR = 20;
          movement.speedL = 20;
          movement.driveDistance(200);
          gettocenter = false;
        }
      }
      for(int i=0; i<7; i++){
        distance1[i] = ultrasonic::getDistance();
      }
      if(getAverage(7,distance1) < 33.0) {
        movement.halt();
        movement.turn(findBeacon(true));
        state = 4;
        gettocenter = true;
      }
      stepCounter = 0;
      i = 0;
      break;
    case 4:
      if (!step.turn_in_progress && !step.arm_in_progress) {
        if(i == 0){
          step.changeStepper();
          step.arm(); 
          i++;
        } else if (i == 1){
          step.changeStepper();
          step.turn(420);
          i++;
        }
        else{
          delay(1000);
          magnet::disableMagnet();
          delay(2000);
          i = 0;
          avg_dist = 0;
          state = 5;
        } 

      }
      delayMicroseconds(700);  
      break;
    case 5:
      if (!movement.turn_in_progress && !movement.driving_set_distance){
        // setMotorSpeed(LEFT_MOTOR, 50);
        if(i == 0) {
          movement.speedL = 20;
          movement.speedR = 20;
          movement.turn(180);
          i++;
        } else if(i == 1) {
          movement.halt();
          movement.driveDistance(40);
          i++;
        } else {
          movement.halt();
          state = 0;
        }
      }
      break;
  }
}