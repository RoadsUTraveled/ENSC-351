// Description: Header file for CartMove.c defining functions for motor driver
// initMotorDriver() - initializes the motor driver
// moveForward() - moves the cart forward
// moveBackward() - moves the cart backward
// turnLeft() - turns the cart left
// turnRight() - turns the cart right
// clearMotorDriver() - clears the motor driver
#ifndef CARTMOVE_H
#define CARTMOVE_H

void initMotorDriver(void);
void moveForward(void);
void moveBackward(void);
void turnLeft(void);
void turnRight(void);
void clearMotorDriver(void);

#endif // CARTMOVE_H