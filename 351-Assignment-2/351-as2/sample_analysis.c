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
bool start = false;
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
void initialSampleAnalysis(void)
{
    pthread_mutex_init(&sampleAnalysisMutex, NULL);
    sampleAnalysisThreadActive = true;
    pthread_create(&sampleAnalysisThread, NULL, sampleAnalysisThreadFunc, NULL);
  
}

// Check Dips with ragard to the threshold
static void checkDips(double newSample)
{
    // When dip is false, it means detected
    if (!sampleAnalysis.dip && (newSample < sampleAnalysis.avgVoltage - DIP_THRESHOLD))
    {
        sampleAnalysis.dip = true;
        sampleAnalysis.numDips++;
    }
    else if (sampleAnalysis.dip && (newSample > sampleAnalysis.avgVoltage - DIP_THRESHOLD + DIP_HYSTERESIS))
    {
        sampleAnalysis.dip = false;
    }
}

void resetAnalysis(void)
{
    sampleAnalysis.minVoltage = DBL_MAX; // Set to maximum double value
    sampleAnalysis.maxVoltage = DBL_MIN; // Set to minimum double value
    sampleAnalysis.avgInterval = 0.0;
    sampleAnalysis.minInterval = DBL_MAX;
    sampleAnalysis.maxInterval = DBL_MIN;
    sampleAnalysis.numDips = 0;
    //sampleAnalysis.dip = false;
}

void updateMinMaxVoltage(double voltage, SampleAnalysis *sampleAnalysis) {
    if (voltage < sampleAnalysis->minVoltage){
         sampleAnalysis->minVoltage = voltage;
    }
    if (voltage > sampleAnalysis->maxVoltage) {
        sampleAnalysis->maxVoltage = voltage;
    }
}

void updateMinMaxInterval(long long currentTimestamp, long long previousTimestamp, SampleAnalysis *sampleAnalysis) {
    double interval = currentTimestamp - previousTimestamp;
    if (interval < sampleAnalysis->minInterval) {
        sampleAnalysis->minInterval = interval;
    }
    if (interval > sampleAnalysis->maxInterval) {
        sampleAnalysis->maxInterval = interval;
    }
}
void updateAverageVoltage(double newVoltage, SampleAnalysis *sampleAnalysis) {
    const double alpha = 0.001; // 0.1% weight for the new sample

    // If it's the first sample, initialize the average with this sample's value
    if (sampleAnalysis->numSamples == 0) {
        sampleAnalysis->avgVoltage = newVoltage;
    } else {
        // Apply exponential averaging
        sampleAnalysis->avgVoltage = alpha * newVoltage + (1 - alpha) * sampleAnalysis->avgVoltage;
    }

    // Increment the number of samples
    //sampleAnalysis->numSamples++;
}


// Analysis of the sample in multiple threads
void *sampleAnalysisThreadFunc(void *arg)
{
    while (sampleAnalysisThreadActive)
    {
        sleep(1);

        // Assuming Sampler_extractAllValues function exists to get all samples
        pthread_mutex_lock(&sampleAnalysisMutex);
        int numSamples;
        samplerDatapoint_t *samples = Sampler_extractAllValues(&numSamples);

        if (samples != NULL){
            resetAnalysis();
            double totalInterval = 0.0;
            sampleAnalysis.numSamples = numSamples;
            for (int i = 0; i < numSamples; ++i) {
                double voltage = samples[i].sampleInV;
                long long timestamp = samples[i].timestampInNanoS;

                updateMinMaxVoltage(voltage, &sampleAnalysis);
                updateAverageVoltage(voltage, &sampleAnalysis);
                checkDips(voltage);

                // Calculate intervals and update min, max, and average interval times
                if (i > 0) {
                    updateMinMaxInterval(timestamp, samples[i - 1].timestampInNanoS, &sampleAnalysis);
                    totalInterval += (timestamp - samples[i - 1].timestampInNanoS);
                }
            }
            if(numSamples > 0){
                sampleAnalysis.avgInterval = totalInterval / (numSamples - 1);
            }
            free(samples);
        }
        else{
            fprintf(stderr, "Error: Failed to extract samples or no samples available.\n");
        }

        // Reset sample Count for the next second
        Sampler_resetSampleCount();
        pthread_mutex_unlock(&sampleAnalysisMutex);
    }
    return NULL;
}

void printSampleAnalysis(void)
{
    pthread_mutex_lock(&sampleAnalysisMutex);

    printf("Interval ms (%.3f, %.3f) avg=%.3f   ",
           sampleAnalysis.minInterval / 1000.0, // Converting nanoseconds to milliseconds
           sampleAnalysis.maxInterval / 1000.0,
           sampleAnalysis.avgInterval / 1000.0);

    printf("Samples V (%.3f, %.3f) avg=%.3f   ",
           sampleAnalysis.minVoltage,
           sampleAnalysis.maxVoltage,
           sampleAnalysis.avgVoltage);

    printf("# Dips: %d   # Samples: %d",
           sampleAnalysis.numDips,
           sampleAnalysis.numSamples);

    printf("\n");

    pthread_mutex_unlock(&sampleAnalysisMutex);
}

// Getters for the sample analysis
double getMinVoltage(void)
{
    pthread_mutex_lock(&sampleAnalysisMutex);
    double minVoltage = sampleAnalysis.minVoltage;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return minVoltage;
}

double getMaxVoltage(void)
{
    pthread_mutex_lock(&sampleAnalysisMutex);
    double maxVoltage = sampleAnalysis.maxVoltage;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return maxVoltage;
}

double getMinInterval(void)
{
    pthread_mutex_lock(&sampleAnalysisMutex);
    double minInterval = sampleAnalysis.minInterval/1000.0;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return minInterval;
}

double getMaxInterval(void)
{
    pthread_mutex_lock(&sampleAnalysisMutex);
    double maxInterval = sampleAnalysis.maxInterval/1000.0;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return maxInterval;
}
int getNumDips(void)
{
    pthread_mutex_lock(&sampleAnalysisMutex);
    int numOfDips = sampleAnalysis.numDips;
    pthread_mutex_unlock(&sampleAnalysisMutex);
    return numOfDips;
}
