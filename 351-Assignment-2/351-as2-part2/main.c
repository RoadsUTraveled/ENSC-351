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

    /*
    Sampler_startSampling();
    resetSampleAnalysis();
    //LED test
    double joyX, joyY;
    Joystick_readXY(&joyX, &joyY);
    joystick_display(joyX, joyY);
  
    //user button 
    controlButtonMonitorThread();*/
    printf("Program exiting smoothly.\n");

    while (true) {
        double x, y;
        Joystick_readXY(&x, &y);
        printf("x: %f, y: %f\n", x, y);
    }

   
    return 0;
}