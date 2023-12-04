
/*
#include "sampler.h"
#include <stdio.h>
#include <unistd.h> // For sleep function
#include <stdlib.h> // For free function
#include "common.h"

int main() {
    // Start the sampling process
    long long startTimeInMicros = getTimeInMicros();
    printf("Starting sampling...\n");
    Sampler_startSampling();

    // Allow some time for sampling
    sleep(1); // Sleep for 5 seconds to collect some samples

    // Extract and print the collected samples
    int length;
    samplerDatapoint_t* samples = Sampler_extractAllValues(&length);
    printf("Collected %d samples:\n", length);
    // Record the start time
    

    for (int i = 0; i < length; ++i) { 
        long long relativeTime = samples[i].timestampInNanoS - startTimeInMicros;
        printf("Sample %d: Voltage = %.3f V, Timestamp = %lld us (relative)\n", 
           i, samples[i].sampleInV, relativeTime);
    }

    // Free the allocated memory
    free(samples);

    // Stop the sampling process
    printf("Stopping sampling...\n");
    Sampler_stopSampling();

    return 0;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include "common.h"
#include "user_button_exit.h"
#include "segDisplay.h"
#include <stdbool.h> // For bool, true, false
#include <unistd.h>  // For usle
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h> 
#include "sampler.h"
#include "sample_analysis.h"

int main() {

    runCommand("config-pin p8.43 gpio");

    // Initialize and start the sampling and analysis threads
    Sampler_startSampling();
    initialSampleAnalysis();

    printf("Starting to sample data...\n");
    /*
    while(1)
    {
        printSampleAnalysis();
        int buttonState = readButton();
        if (buttonState == 0) {
            printf("pressed button\n");
            ledClose();
            break;
        }
        struct timespec req, rem;
        req.tv_sec = 0;
        req.tv_nsec = 100000000L; // 100 ms in nanoseconds
        nanosleep(&req, &rem);
    }
    */
    while(1){
        printSampleAnalysis();
        sleep(1);
        printf("\n");
    }

    // Stop the sampling and analysis processes
    Sampler_stopSampling();
    cleanup_sample_analysis();

    // Print the analysis results
    printf("Shutting down...\n");
    printSampleAnalysis();

    return 0;
}

