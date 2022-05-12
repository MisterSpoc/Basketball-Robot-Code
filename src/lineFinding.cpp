#include "lineFinding.h"

//using namespace lineSensing;
    uint16_t sensorVal[LS_NUM_SENSORS];
    uint16_t sensorCalVal[LS_NUM_SENSORS];
    uint16_t sensorMaxVal[LS_NUM_SENSORS];
    uint16_t sensorMinVal[LS_NUM_SENSORS];
    uint8_t lineColor = DARK_LINE;

    uint16_t pre_calibration_min[] = {653,802,574,694,585,664,649,696};
    uint16_t pre_calibration_max[] = {730,883,649,775,622,728,699,759};

void lineSensing::setup() {
    clearMinMax(sensorMinVal, sensorMaxVal);
    for(int i=0; i<LS_NUM_SENSORS; i++){
        sensorMinVal[i] = pre_calibration_min[i];
        sensorMaxVal[i] = pre_calibration_max[i];
    }
    // initialize sensorMinVal and sensorMaxVal here based on pre calibration numbers
    
}

void lineSensing::checks() {
    readLineSensor(sensorVal);
    readCalLineSensor(sensorVal, sensorCalVal, sensorMinVal, sensorMaxVal, lineColor);
}

bool lineSensing::atIntersection() {
  for(int i = 0; i < LS_NUM_SENSORS; i++) {
    if(sensorVal[i] != 2500) {
      return false;
    }
  }
  return true;
}

bool lineSensing::onLine() {
  uint32_t value = getLinePosition(sensorCalVal, lineColor);
  if(value < 1000) {
    return false;
  }
  else{
    return true;
  }
}

uint32_t lineSensing::linePos() {
  return getLinePosition(sensorCalVal, lineColor);
}

void lineSensing::followLinePID(){
  float Kp = 4.5; //set up the constants value
  float Ki = 3;
  float Kd = 0.0;
  int P;
  int I;
  int D;

  int lastError = 0;
  boolean onoff = false;

  //Increasing the maxspeed can damage the motors - at a value of 255 the 6V motors will receive 7,4 V 
  const uint8_t max_speed_R = 50;
  const uint8_t max_speed_L = 50;
  const uint8_t base_speed_R = 15;
  const uint8_t base_speed_L = 15;

  uint32_t position = getLinePosition(sensorCalVal,lineColor);
  int error = 3500 - position;

  P = error;
  I = I + error;
  D = error - lastError;
  lastError = error;
  int motor_speed = P*Kp + I*Ki + D*Kd;
  
  int motor_speed_R = base_speed_R - motor_speed;
  int motor_speed_L = base_speed_L + motor_speed;
  
  if (motor_speed_R > max_speed_R) {
    motor_speed_R = max_speed_R;
  }
  if (motor_speed_L > max_speed_L) {
    motor_speed_L = max_speed_L;
  }
  if (motor_speed_R < 0) {
    motor_speed_R = 0;
  }
  if (motor_speed_L < 0) {
    motor_speed_L = 0;
  } 

  setMotorSpeed(LEFT_MOTOR, motor_speed_L);
	setMotorSpeed(RIGHT_MOTOR, motor_speed_R);
}