#include "sample_analysis.h"
#include "common.h"
#include "sampler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <float.h>
#include <math.h>  // Add this line


typedef struct {
    double avgVoltage;
    double minVoltage;
    double maxVoltage;
    double avgInterval;
    double minInterval;
    double maxInterval;
    int numSamples;
    int numDips;
    bool dip;
} SampleAnalysis;

static SampleAnalysis sampleAnalysis;
static pthread_mutex_t sampleAnalysisMutex;
static pthread_t sampleAnalysisThread;
static bool sampleAnalysisThreadActive = false;
static const double DIP_THRESHOLD = 0.1;
static const double DIP_HYSTERESIS = 0.03;
// Thread initialization and cleanup functions

void cleanup_sample_analysis(void)
{
    sampleAnalysisThreadActive = false;
    pthread_join(sampleAnalysisThread, NULL);
    pthread_mutex_destroy(&sampleAnalysisMutex);
}

//Helper function for voltage
double findMaxVoltage(samplerDatapoint_t* samples, int numSamples) {
    double maxVoltage = -DBL_MAX;
    for (int i = 0; i < numSamples; ++i) {
        if (samples[i].sampleInV > maxVoltage) {
            maxVoltage = samples[i].sampleInV;
        }
    }
    return maxVoltage;
}
double findMinVoltage(samplerDatapoint_t* samples, int numSamples) {
    double minVoltage = DBL_MAX;
    for (int i = 0; i < numSamples; ++i) {
        if (samples[i].sampleInV < minVoltage) {
            minVoltage = samples[i].sampleInV;
        }
    }
    return minVoltage;
}
double findAvgVoltage(samplerDatapoint_t* samples, int numSamples) {
    double totalVoltage = 0.0;
    for (int i = 0; i < numSamples; ++i) {
        totalVoltage += samples[i].sampleInV;
    }
    return numSamples > 0 ? totalVoltage / numSamples : 0.0;
}


//Helper function for time Interval
double findMaxInterval(samplerDatapoint_t* samples, int numSamples) {
    double maxInterval = -DBL_MAX;
    for (int i = 1; i < numSamples; ++i) {
        double interval = samples[i].timestampInNanoS - samples[i - 1].timestampInNanoS;
        maxInterval = fmax(maxInterval, interval);
    }
    return maxInterval;
}

double findMinInterval(samplerDatapoint_t* samples, int numSamples) {
    double minInterval = DBL_MAX;
    for (int i = 1; i < numSamples; ++i) {
        double interval = samples[i].timestampInNanoS - samples[i - 1].timestampInNanoS;
        minInterval = fmin(minInterval, interval);
    }
    return minInterval;
}

double findAvgInterval(samplerDatapoint_t* samples, int numSamples) {
    double totalInterval = 0.0;
    for (int i = 1; i < numSamples; ++i) {
        totalInterval += samples[i].timestampInNanoS - samples[i - 1].timestampInNanoS;
    }
    return (numSamples > 1) ? totalInterval / (numSamples - 1) : 0.0;
}


//printer ffuntions
void printSampleAnalysis(void);

//Update Average Voltage
static void updateAvgVoltage(double newVoltage) {
    if (sampleAnalysis.numSamples == 0) {
        sampleAnalysis.avgVoltage = newVoltage;
    } else {
        sampleAnalysis.avgVoltage = (sampleAnalysis.avgVoltage * sampleAnalysis.numSamples + newVoltage) / (sampleAnalysis.numSamples + 1);
    }
}

//Check Dips with ragard to the threshold
static void checkDips(double newSample) {
    if (!sampleAnalysis.dip && newSample < sampleAnalysis.avgVoltage - DIP_THRESHOLD) {
        sampleAnalysis.dip = true;
        sampleAnalysis.numDips++;
    } else if (sampleAnalysis.dip && newSample > sampleAnalysis.avgVoltage - DIP_THRESHOLD + DIP_HYSTERESIS) {
        sampleAnalysis.dip = false;
    }
}
void init_sample_analysis(void) {
    pthread_mutex_init(&sampleAnalysisMutex, NULL);
    sampleAnalysisThreadActive = true;

    // Initialize the sampleAnalysis struct
    pthread_mutex_lock(&sampleAnalysisMutex);
    sampleAnalysis.avgVoltage = 0.0;
    sampleAnalysis.minVoltage = 0.1;  // Set to maximum double value
    sampleAnalysis.maxVoltage = 1.8; // Set to minimum double value
    sampleAnalysis.avgInterval = 0.0;
    sampleAnalysis.minInterval = 0.0;
    sampleAnalysis.maxInterval = 0.1;
    sampleAnalysis.numSamples = 0;
    sampleAnalysis.numDips = 0;
    sampleAnalysis.dip = false;
    pthread_mutex_unlock(&sampleAnalysisMutex);

    pthread_create(&sampleAnalysisThread, NULL, sampleAnalysisThreadFunc, NULL);
}

//Analysis of the sample in multiple threads
void* sampleAnalysisThreadFunc(void* arg) {
    
    while (sampleAnalysisThreadActive) {
        sleep(1);
        pthread_mutex_lock(&sampleAnalysisMutex);

        // Assuming Sampler_extractAllValues function exists to get all samples
        int numSamples;
        samplerDatapoint_t* samples = Sampler_extractAllValues(&numSamples);

        // Compute statistics
        sampleAnalysis.minVoltage = findMinVoltage(samples, numSamples);
        sampleAnalysis.maxVoltage = findMaxVoltage(samples, numSamples);
        sampleAnalysis.avgVoltage = findAvgVoltage(samples, numSamples);
        sampleAnalysis.minInterval = findMinInterval(samples, numSamples);
        sampleAnalysis.maxInterval = findMaxInterval(samples, numSamples);
        sampleAnalysis.avgInterval = findAvgInterval(samples, numSamples);
        sampleAnalysis.numSamples = numSamples;

        // Check for dips
        for (int i = 0; i < numSamples; ++i) {
            updateAvgVoltage(samples[i].sampleInV);
            checkDips(samples[i].sampleInV);
        }

        // Reset sample Count for the next second
        numSamples = 0;
        
        pthread_mutex_unlock(&sampleAnalysisMutex);

        // Print the analysis
        printSampleAnalysis();

    }
    return NULL;
}

void printSampleAnalysis(void) {
    pthread_mutex_lock(&sampleAnalysisMutex);

    printf("Average Voltage: %.3f V\n", sampleAnalysis.avgVoltage);
    printf("Minimum Voltage: %.3f V\n", sampleAnalysis.minVoltage);
    printf("Maximum Voltage: %.3f V\n", sampleAnalysis.maxVoltage);
    printf("Average Interval: %.3f us\n", sampleAnalysis.avgInterval);
    printf("Minimum Interval: %.3f us\n", sampleAnalysis.minInterval);
    printf("Maximum Interval: %.3f us\n", sampleAnalysis.maxInterval);
    printf("Number of Samples: %d\n", sampleAnalysis.numSamples);
    printf("Number of Dips: %d\n", sampleAnalysis.numDips);

    pthread_mutex_unlock(&sampleAnalysisMutex);
}

