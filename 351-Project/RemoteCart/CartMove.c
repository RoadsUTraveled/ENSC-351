#include "CartMove.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

#define IN1 62
#define IN2 36
#define IN3 32
#define IN4 86

void initialize()
{
    // Export GPIO pins and set them as outputs
    exportGPIO(IN1);
    exportGPIO(IN2);
    exportGPIO(IN3);
    exportGPIO(IN4);

    setDirection(IN1, "out");
    setDirection(IN2, "out");
    setDirection(IN3, "out");
    setDirection(IN4, "out");
}

void moveForward()
{
    // Move forward
    setGPIO(IN1, 1);
    setGPIO(IN2, 0);
    setGPIO(IN3, 1);
    setGPIO(IN4, 0);
}

void moveBackward()
{
    // Move backward
    setGPIO(IN1, 0);
    setGPIO(IN2, 1);
    setGPIO(IN3, 0);
    setGPIO(IN4, 1);
}

void turnLeft()
{
    // Turn left
    setGPIO(IN1, 0);
    setGPIO(IN2, 1);
    setGPIO(IN3, 1);
    setGPIO(IN4, 0);
}

void turnRight()
{
    // Turn right
    setGPIO(IN1, 1);
    setGPIO(IN2, 0);
    setGPIO(IN3, 0);
    setGPIO(IN4, 1);
}

void clearMotorDriver()
{
    // Clear motor driver
    setGPIO(IN1, 0);
    setGPIO(IN2, 0);
    setGPIO(IN3, 0);
    setGPIO(IN4, 0);
}
