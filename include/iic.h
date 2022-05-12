#ifndef iic_h
#define iic_h

#include <Wire.h>
#define Arduino 8 //Arduino Nano needs to be set up as slave with address 8

/// \brief sets up MSP432 as iic master device
void IicSetup();

/// \brief returns status of shooting mechanism
/// \param[out] message an 8 bit unsigned integer representing arduino status
uint8_t getStatus();

/// \brief sends message to arduino
/// \param[in] message[] message to be sent formatted as char[]
void send(char message[]);

#endif