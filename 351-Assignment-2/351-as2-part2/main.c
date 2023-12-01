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
    Sampler_startSampling();
    resetSampleAnalysis();
    //LED test
    int i2cFileDesc = ledOpen(9.8);
  
    //user button 
    controlButtonMonitorThread();
    close(i2cFileDesc);
    printf("Program exiting smoothly.\n");
    /**********************test for joystick******************/

    // while (true) {
    //     double x, y;
    //     Joystick_readXY(&x, &y);
    //     printf("x: %f, y: %f\n", x, y);
    // }

    return 0;
}