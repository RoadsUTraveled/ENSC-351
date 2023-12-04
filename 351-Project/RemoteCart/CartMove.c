#include "CartMove.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>

#define IN1 62
#define IN2 36
#define IN3 32
#define IN4 86

void initMotorDriver()
{
    // Export GPIO pins and set them as outputs
    exportGPIO(IN1);
    exportGPIO(IN2);
    exportGPIO(IN3);
    exportGPIO(IN4);

    setGPIO_Direction(IN1, "out");
    setGPIO_Direction(IN2, "out");
    setGPIO_Direction(IN3, "out");
    setGPIO_Direction(IN4, "out");
}

void moveForward()
{
    // Move forward
    setGPIO_Value(IN1, 1);
    setGPIO_Value(IN2, 0);
    setGPIO_Value(IN3, 1);
    setGPIO_Value(IN4, 0);
}

void moveBackward()
{
    // Move backward
    setGPIO_Value(IN1, 0);
    setGPIO_Value(IN2, 1);
    setGPIO_Value(IN3, 0);
    setGPIO_Value(IN4, 1);
}

void turnLeft()
{
    // Turn left
    setGPIO_Value(IN1, 0);
    setGPIO_Value(IN2, 1);
    setGPIO_Value(IN3, 1);
    setGPIO_Value(IN4, 0);
}

void turnRight()
{
    // Turn right
    setGPIO_Value(IN1, 1);
    setGPIO_Value(IN2, 0);
    setGPIO_Value(IN3, 0);
    setGPIO_Value(IN4, 1);
}

void clearMotorDriver()
{
    // Clear motor driver
    setGPIO_Value(IN1, 0);
    setGPIO_Value(IN2, 0);
    setGPIO_Value(IN3, 0);
    setGPIO_Value(IN4, 0);

    unexportGPIO(IN1);
    unexportGPIO(IN2);
    unexportGPIO(IN3);
    unexportGPIO(IN4);
}
