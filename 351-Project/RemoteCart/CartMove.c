#include "CartMove.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define IN1 66
#define IN2 67
#define IN3 69
#define IN4 68

void initMotorDriver()
{
    printf("Exporting GPIO pins...\n");
    // Export GPIO pins
    exportGPIO(IN1);
    exportGPIO(IN2);
    exportGPIO(IN3);
    exportGPIO(IN4);
    printf("GPIO pins exported.\n");

    sleep(1); // Wait for 1 second

    printf("Setting GPIO direction...\n");

    // Set GPIO direction
    setGPIODirection(IN1, "out");
    setGPIODirection(IN2, "out");
    setGPIODirection(IN3, "out");
    setGPIODirection(IN4, "out");
}

void moveForward()
{
    writeGPIO(IN1, 1);
    writeGPIO(IN2, 0);
    writeGPIO(IN3, 1);
    writeGPIO(IN4, 0);
}

void moveBackward()
{
    writeGPIO(IN1, 0);
    writeGPIO(IN2, 1);
    writeGPIO(IN3, 0);
    writeGPIO(IN4, 1);
}

void turnLeft()
{
    writeGPIO(IN1, 0);
    writeGPIO(IN2, 1);
    writeGPIO(IN3, 1);
    writeGPIO(IN4, 0);
}

void turnRight()
{
    writeGPIO(IN1, 1);
    writeGPIO(IN2, 0);
    writeGPIO(IN3, 0);
    writeGPIO(IN4, 1);
}

void clearMotorDriver()
{
    // Clear motor driver
    writeGPIO(IN1, 0);
    writeGPIO(IN2, 0);
    writeGPIO(IN3, 0);
    writeGPIO(IN4, 0);

    unexportGPIO(IN1);
    unexportGPIO(IN2);
    unexportGPIO(IN3);
    unexportGPIO(IN4);
}
