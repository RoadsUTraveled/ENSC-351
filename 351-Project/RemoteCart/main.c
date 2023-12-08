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
#include "segDisplay.h"

int main()
{
    /************************ TEST CODE FOR CartMove ************************/
    initMotorDriver();
    int i2cFileDesc = ledInitialize();
    //sleep(10);
    
    int ControlSignal = 0;
    while(ControlSignal == 0)
    {
        readGPIO(45, &ControlSignal);
        printf("value = %d\n", ControlSignal);
    }
    
    // Initialize the motor driver
    
    while (1)
    {
        printf("value = %d\n", ControlSignal);
        readGPIO(45, &ControlSignal);

        if (ControlSignal == 0)
            break;
        // Read the control signal from ESP32
        int ESP32_IN1 = 0;
        int ESP32_IN2 = 0;
        readGPIO(IN1, &ESP32_IN1);
        readGPIO(IN2, &ESP32_IN2);
        
        // Comment for debug
        printf("ESP32_IN1 = %d\n", ESP32_IN1);
        printf("ESP32_IN2 = %d\n", ESP32_IN2);
        printf("value = %d\n", ControlSignal);
        displayPatternBasedOnInput(i2cFileDesc, ESP32_IN1, ESP32_IN2);   
            // Move the cart
        cartMovement(ESP32_IN1, ESP32_IN2);
         
    }
    // Clear the motor driver
    clearMotorDriver();
    cleanLed(i2cFileDesc);
    close(i2cFileDesc);
    printf("clean finish\n");
    printf("value = %d\n", ControlSignal);
    

   /*
    displayPatternBasedOnInput(i2cFileDesc, 0, 1);
    sleep(5);
    cleanLed(i2cFileDesc);
    close(i2cFileDesc);*/
    return 0;
}