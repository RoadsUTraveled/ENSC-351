#include "CartMove.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void moveForward()
{
    writeGPIO(OUT1, 1);
    writeGPIO(OUT2, 0);
    writeGPIO(OUT3, 1);
    writeGPIO(OUT4, 0);
}

void moveBackward()
{
    writeGPIO(OUT1, 0);
    writeGPIO(OUT2, 1);
    writeGPIO(OUT3, 0);
    writeGPIO(OUT4, 1);
}

void turnLeft()
{
    writeGPIO(OUT1, 0);
    writeGPIO(OUT2, 1);
    writeGPIO(OUT3, 1);
    writeGPIO(OUT4, 0);
}

void turnRight()
{
    writeGPIO(OUT1, 1);
    writeGPIO(OUT2, 0);
    writeGPIO(OUT3, 0);
    writeGPIO(OUT4, 1);
}

void stop(){
    writeGPIO(OUT1, 0);
    writeGPIO(OUT2, 0);
    writeGPIO(OUT3, 0);
    writeGPIO(OUT4, 0);
}
void initMotorDriver()
{
    //printf("Exporting GPIO pins...\n");
    // Export GPIO pins
    exportGPIO(OUT1);
    exportGPIO(OUT2);
    exportGPIO(OUT3);
    exportGPIO(OUT4);
    exportGPIO(IN1);
    exportGPIO(IN2);
    exportGPIO(45);
    //Comment for debugg
    //printf("GPIO pins exported.\n");
    sleep(1); // Wait for 1 second
    //printf("Setting GPIO direction...\n");
    
    // Set GPIO direction
     setGPIODirection(OUT1, "out");
     setGPIODirection(OUT2, "out");
     setGPIODirection(OUT3, "out");
     setGPIODirection(OUT4, "out");
     setGPIODirection(IN1, "in");
     setGPIODirection(IN2, "in");
     setGPIODirection(45, "in");
}

void cartMovement(int ESP32_IN1, int ESP32_IN2)
{
    //printf("Setting GPIO value...\n");
    // Set GPIO value
    if (ESP32_IN1 == 0 && ESP32_IN2 == 0)
    {
        moveForward();
    }
    else if (ESP32_IN1 == 1 && ESP32_IN2 == 1)
    {
        moveBackward();
    }
    else if (ESP32_IN1 == 1 && ESP32_IN2 == 0)
    {
        turnLeft();
    }
    else if (ESP32_IN1 == 0 && ESP32_IN2 == 1)
    {
        turnRight();
    }
}
void carstop(){
    stop();
}

void clearMotorDriver()
{
    // Clear motor driver
    writeGPIO(OUT1, 0);
    writeGPIO(OUT2, 0);
    writeGPIO(OUT3, 0);
    writeGPIO(OUT4, 0);
    setGPIODirection(45,"out");
    sleep(1);
    writeGPIO(45, 0);
    unexportGPIO(OUT1);
    unexportGPIO(OUT2);
    unexportGPIO(OUT3);
    unexportGPIO(OUT4);
    unexportGPIO(IN1);
    unexportGPIO(IN2);
    unexportGPIO(45);
}
