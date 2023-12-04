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
#include "time.h"


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
 
    printf("Please press the button for one second to close!\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    //start 
    Sampler_startSampling();
    initialSampleAnalysis();
    

    sleep(0.1);
    
    //user button 
    controlButtonMonitorThread();
    
    printf("Program exiting smoothly.\n");
    return 0;
}