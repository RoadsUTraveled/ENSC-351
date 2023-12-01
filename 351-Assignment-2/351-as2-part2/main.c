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

    // Initialize the mutex
    //pthread_mutex_init(&mutex, NULL);

    //LED test
    int i2cFileDesc = ledOpen(0.8);

    /*
    int i2cFileDesc = ledInitialize();
    // Initialize the thread argument
    ThreadArg threadArg;
    threadArg.number = 8.8;  // Change the number here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
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
    printf("Program exiting smoothly.\n");

   
    return 0;
}