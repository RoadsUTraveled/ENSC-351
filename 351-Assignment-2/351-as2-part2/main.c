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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <unistd.h> 

int main() {
    // test the A2D functions
    /*
    while (true) {
        int reading = getVoltageReading(3);
        double voltage = getVoltage(3);
        printf("Reading: %d, Voltage: %f\n", reading, voltage);
    }*/

    //LED test
    int i2cFileDesc = ledInitialize();
    // Initialize the thread argument
    ThreadArg threadArg;
    threadArg.number = 100;  // Change the number here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
    threadArg.i2cFileDesc = i2cFileDesc;

    // Create the display thread
    pthread_t displayThread;
    if (pthread_create(&displayThread, NULL, displayThreadFunc, &threadArg)) {
        fprintf(stderr, "Error creating display thread\n");
        return 1;
    }

    // Wait for the thread to finish
    pthread_join(displayThread, NULL);

    // Cleanup I2C access
    close(i2cFileDesc);
    
    return 0;
}