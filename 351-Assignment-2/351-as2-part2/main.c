#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Include all the header files
#include "common.h"
#include "display.h"
#include "Joystick_Readings.h"
#include "sample_analysis.h"
#include "sampler.h"
#include "user_button_exit.h"
#include "A2D.h"
#include "segDisplay.h"
#include "user_button_exit.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <unistd.h> 


//pthread_mutex_t mutex; // Declare the mutex globally

int main() {
    // test the A2D functions
    /*
    while (true) {
        int reading = getVoltageReading(3);
        double voltage = getVoltage(3);
        printf("Reading: %d, Voltage: %f\n", reading, voltage);
    }*/

<<<<<<< HEAD
    // Initialize the mutex
    //pthread_mutex_init(&mutex, NULL);

    //LED test
    int i2cFileDesc = ledOpen(0.8);

    /*
    int i2cFileDesc = ledInitialize();
    // Initialize the thread argument
    ThreadArg threadArg;
    threadArg.number = 8.8;  // Change the number here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
=======

    //test for LED
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");
    
    runCommand("config-pin p8.26 gpio");
    runCommand("config-pin p8.12 gpio");
    runCommand("cd /sys/class/gpio");
    sleep(1);
    runCommand("echo 61 > /sys/class/gpio/export");
    sleep(1);
    runCommand("echo out > /sys/class/gpio/gpio61/direction");
    sleep(1);
    runCommand("echo 44 > /sys/class/gpio/export");
    sleep(1);
    runCommand("echo out > /sys/class/gpio/gpio44/direction");

	printf("Drive display (assumes GPIO #61 and #44 are output and 1\n");
	int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    
	writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
	writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);

	// Drive an hour-glass looking character (Like an X with a bar on top & bottom)
	writeI2cReg(i2cFileDesc, REG_OUTA, 0x2A);
	writeI2cReg(i2cFileDesc, REG_OUTB, 0x54);
    

	// Read a register:
	unsigned char regVal = readI2cReg(i2cFileDesc, REG_OUTA);
	printf("Reg OUT-A = 0x%02x\n", regVal);

    writeI2cReg(i2cFileDesc, 0x21, 0x00);
    writeI2cReg(i2cFileDesc, 0x81, 0x00);

    // Initialize the thread argument
    ThreadArg threadArg;
    threadArg.number = 0;  // Change the number here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! (work for only int now)
>>>>>>> 00d61d88f677308d9c97524aecfccb188b7b3920
    threadArg.i2cFileDesc = i2cFileDesc;

    // Create the display thread
    pthread_t displayThread, buttonMonitorThread;
    if (pthread_create(&displayThread, NULL, displayThreadFunc, &threadArg)) {
        fprintf(stderr, "Error creating display thread\n");
        return 1;
    }
    pthread_create(&buttonMonitorThread, NULL, buttonMonitorThreadFunc, NULL);*/

    // Wait for the thread to finish
    //pthread_join(displayThread, NULL);
    //pthread_join(buttonMonitorThread, NULL);
    // Cleanup I2C access
    //sleep(5);
    //ledClose();
    controlButtonMonitorThread();
    close(i2cFileDesc);
<<<<<<< HEAD
    printf("Program exiting smoothly.\n");

   
=======


    /**********************test for joystick******************/

    // while (true) {
    //     double x, y;
    //     Joystick_readXY(&x, &y);
    //     printf("x: %f, y: %f\n", x, y);
    // }
    
>>>>>>> 00d61d88f677308d9c97524aecfccb188b7b3920
    return 0;
}