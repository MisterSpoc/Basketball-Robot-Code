#include "SimpleRSLK.h"
#include "Sensing.h"

Sensing::Sensing(){}

bool Sensing::bumper_sensors[];

void Sensing::initialize(){
  // initialize IR pins
  
  pinMode(IR0, INPUT_PULLUP); // make on-board pushbutton's pin an input pullup
  pinMode(IR1, INPUT_PULLUP); // 1 = no beacon detected, 0 = beacon detected
  pinMode(IR2, INPUT_PULLUP);
}

int Sensing::checkIR(){ 
  // 1 = no beacon detected, 0 = beacon detected
  int IR_PINS[] = {IR0, IR1, IR2}; // assign beacons to array
  int IRState;
  uint8_t sensor_output = 0; 
  for (int i = 0; i <= 2; i++){
    sensor_output += digitalRead(IR_PINS[i])<<i;
  }
  return sensor_output;
}

 void Sensing::checkBumpers() {
  /* 
   *  checks bumper switches, edits bumper_sensors array 1 = on 0 = off
   */
  for (int i = 0; i < BUMPER_SENSORS; i++){
    bumper_sensors[i] = !digitalRead(BUMPER_PINS[i]);
  }
 }
  
