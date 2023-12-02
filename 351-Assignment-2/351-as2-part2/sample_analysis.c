#include "sample_analysis.h"
#include "common.h"
#include "sampler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <float.h>
#include <math.h> // Add this line

typedef struct
{
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

// Helper function for voltage
double findMaxVoltage(samplerDatapoint_t *samples, int numSamples)
{
    double maxVoltage = -DBL_MAX;
    for (int i = 0; i < numSamples; ++i)
    {
        if (samples[i].sampleInV > maxVoltage)
        {
            maxVoltage = samples[i].sampleInV;
        }
    }
    return maxVoltage;
}
double findMinVoltage(samplerDatapoint_t *samples, int numSamples)
{
    double minVoltage = DBL_MAX;
    for (int i = 0; i < numSamples; ++i)
    {
        if (samples[i].sampleInV < minVoltage)
        {
            minVoltage = samples[i].sampleInV;
        }
    }
    return minVoltage;
}
double findAvgVoltage(samplerDatapoint_t *samples, int numSamples)
{
    double totalVoltage = 0.0;
    for (int i = 0; i < numSamples; ++i)
    {
        totalVoltage += samples[i].sampleInV;
    }
    return numSamples > 0 ? totalVoltage / numSamples : 0.0;
}

// Helper function for time Interval
double findMaxInterval(samplerDatapoint_t *samples, int numSamples)
{
    double maxInterval = -DBL_MAX;
    for (int i = 1; i < numSamples; ++i)
    {
        double interval = samples[i].timestampInNanoS - samples[i - 1].timestampInNanoS;
        maxInterval = fmax(maxInterval, interval);
    }
    return maxInterval;
}

double findMinInterval(samplerDatapoint_t *samples, int numSamples)
{
    double minInterval = DBL_MAX;
    for (int i = 1; i < numSamples; ++i)
    {
        double interval = samples[i].timestampInNanoS - samples[i - 1].timestampInNanoS;
        minInterval = fmin(minInterval, interval);
    }
    return minInterval;
}

double findAvgInterval(samplerDatapoint_t *samples, int numSamples)
{
    double totalInterval = 0.0;
    for (int i = 1; i < numSamples; ++i)
    {
        totalInterval += samples[i].timestampInNanoS - samples[i - 1].timestampInNanoS;
    }
    return (numSamples > 1) ? totalInterval / (numSamples - 1) : 0.0;
}

// Update Average Voltage
/*
static void updateAvgVoltage(double newVoltage)
{
    if (sampleAnalysis.numSamples == 0)
    {
        sampleAnalysis.avgVoltage = newVoltage;
    }
    else
    {
        sampleAnalysis.avgVoltage = (sampleAnalysis.avgVoltage * sampleAnalysis.numSamples + newVoltage) / (sampleAnalysis.numSamples + 1);
    }
}
*/

// Check Dips with ragard to the threshold
static void checkDips(double newSample)
{
    //When dip is false, it means detected
    if (!sampleAnalysis.dip && newSample < sampleAnalysis.avgVoltage - DIP_THRESHOLD)
    {
        sampleAnalysis.dip = true;
        sampleAnalysis.numDips++;
    }
    else if (sampleAnalysis.dip && newSample > sampleAnalysis.avgVoltage - DIP_THRESHOLD + DIP_HYSTERESIS)
    {
        sampleAnalysis.dip = false;
    }
}
void resetSampleAnalysis(void)
{
    pthread_mutex_init(&sampleAnalysisMutex, NULL);
    sampleAnalysisThreadActive = true;

    // Initialize the sampleAnalysis struct
    pthread_mutex_lock(&sampleAnalysisMutex);
    sampleAnalysis.avgVoltage = 0.0;
    sampleAnalysis.minVoltage = DBL_MAX; // Set to maximum double value
    sampleAnalysis.maxVoltage = DBL_MIN; // Set to minimum double value
    sampleAnalysis.avgInterval = 0.0;
    sampleAnalysis.minInterval = DBL_MAX;
    sampleAnalysis.maxInterval = DBL_MIN;
    sampleAnalysis.numSamples = 0;
    sampleAnalysis.numDips = 0;
    sampleAnalysis.dip = false;
    pthread_mutex_unlock(&sampleAnalysisMutex);

    pthread_create(&sampleAnalysisThread, NULL, sampleAnalysisThreadFunc, NULL);
}
void computeStatistics(samplerDatapoint_t* samples, int length) 
{
    resetSampleAnalysis();

    sampleAnalysis.avgVoltage = findAvgVoltage(samples, length);
    sampleAnalysis.minVoltage = findMinVoltage(samples, length);
    sampleAnalysis.maxVoltage = findMaxVoltage(samples, length);

    sampleAnalysis.avgInterval = findAvgInterval(samples, length);
    sampleAnalysis.minInterval = findMinInterval(samples, length);
    sampleAnalysis.maxInterval = findMaxInterval(samples, length);

    for (int i = 0; i < length; ++i) {
        checkDips(samples[i].sampleInV);
    }

    sampleAnalysis.numSamples = length;
}


// Analysis of the sample in multiple threads
void *sampleAnalysisThreadFunc(void *arg)
{
    while (sampleAnalysisThreadActive)
    {
        sleep(1);

        // Assuming Sampler_extractAllValues function exists to get all samples
        int numSamples;
        samplerDatapoint_t *samples = Sampler_extractAllValues(&numSamples);

        if(numSamples > 0){
            pthread_mutex_lock(&sampleAnalysisMutex);
            computeStatistics(samples,numSamples);
            pthread_mutex_unlock(&sampleAnalysisMutex);
            printSampleAnalysis();
            printf("\n");
        }
        // Reset sample Count for the next second
        Sampler_resetSampleCount();

        free(samples);
    }
    return NULL;
}

void printSampleAnalysis(void)
{
    pthread_mutex_lock(&sampleAnalysisMutex);

    printf("Interval ms (%.3f, %.3f) avg=%.3f   ", 
           sampleAnalysis.minInterval / 1000.0,  // Converting nanoseconds to milliseconds
           sampleAnalysis.maxInterval / 1000.0, 
           sampleAnalysis.avgInterval / 1000.0);

    printf("Samples V (%.3f, %.3f) avg=%.3f   ", 
           sampleAnalysis.minVoltage, 
           sampleAnalysis.maxVoltage, 
           sampleAnalysis.avgVoltage);

    printf("# Dips: %d   # Samples: %d", 
           sampleAnalysis.numDips, 
           sampleAnalysis.numSamples);

    pthread_mutex_unlock(&sampleAnalysisMutex);
}

// Getters for the sample analysis
double getMinVoltage(void) {
    pthread_mutex_lock(&sampleAnalysisMutex);
    double minVoltage = sampleAnalysis.minVoltage;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return minVoltage;
}

double getMaxVoltage(void) {
    pthread_mutex_lock(&sampleAnalysisMutex);
    double maxVoltage = sampleAnalysis.maxVoltage;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return maxVoltage;
}

double getMinInterval(void) {
    pthread_mutex_lock(&sampleAnalysisMutex);
    double minInterval = sampleAnalysis.minInterval;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return minInterval;
}

double getMaxInterval(void) {
    pthread_mutex_lock(&sampleAnalysisMutex);
    double maxInterval = sampleAnalysis.maxInterval;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return maxInterval;
}


