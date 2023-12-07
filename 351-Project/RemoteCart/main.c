#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// Inclue header files here
#include "common.h"
#include "CartMove.h"

int main()
{
/************************ TEST CODE FOR CartMove ************************/
    // Initialize the motor driver
    initMotorDriver();

    // Read the control signal from ESP32
    int ESP32_IN1 = 0;
    int ESP32_IN2 = 0;
    readGPIO(IN1, &ESP32_IN1);
    readGPIO(IN2, &ESP32_IN2);

    // Comment for debug
    printf("ESP32_IN1 = %d\n", ESP32_IN1);
    printf("ESP32_IN2 = %d\n", ESP32_IN2);

    // Move the cart
    cartMovement(ESP32_IN1, ESP32_IN2);

    // Clear the motor driver
    clearMotorDriver();
/************************ END OF TEST CODE FOR CartMove ************************/
    return 0;
}