// Description: Header file for CartMove.c defining functions for motor driver

#ifndef CARTMOVE_H
#define CARTMOVE_H

//Control signal from ESP32
#define IN1 48
#define IN2 49

//Control signal sent to motor driver
#define OUT1 66
#define OUT2 67
#define OUT3 69
#define OUT4 68

void initMotorDriver(void);
void cartMovement(int ESP32_IN1, int ESP32_IN2);
void carstop();
void clearMotorDriver(void);

#endif // CARTMOVE_H