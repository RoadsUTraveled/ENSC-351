
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
}*/

#include "sampler.h"
#include "sample_analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep

int main() {
    // Initialize the sampler
    Sampler_startSampling();  // Assuming this function exists in your sampler module

    // Initialize the sample analysis
    init_sample_analysis();

    // Allow some time for sampling and analysis
    // This will give time for the sampling and analysis threads to run
    printf("Sampling and analyzing for 10 seconds...\n");
    sleep(10);


    // Cleanup the sample analysis
    cleanup_sample_analysis();

    // Stop the sampler
    Sampler_stopSampling();  // Assuming this function exists in your sampler module

    printf("Sampling and analysis completed.\n");

    return 0;
}


