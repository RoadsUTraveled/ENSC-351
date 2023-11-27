#include "sample_analysis.h"
#include "common.h"
#include "sampler.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <float.h>

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

//Analysis of the sample in multiple threads
static void* sampleAnalysisThreadFunc(void* arg) {
    while (sampleAnalysisThreadActive) {
        pthread_mutex_lock(&sampleAnalysisMutex);
        samplerDatapoint_t* samples;
        int numSamples;
        samples = Sampler_extractAllValues(&numSamples);
        pthread_mutex_unlock(&sampleAnalysisMutex);
        for (int i = 0; i < numSamples; i++) {
            updateAvgVoltage(samples[i].sampleInV);
            checkDips(samples[i].sampleInV);
            if (i > 0) {
                double interval = (samples[i].timestampInNanoS - samples[i - 1].timestampInNanoS) / 1000000000.0;
                if (sampleAnalysis.numSamples == 0) {
                    sampleAnalysis.avgInterval = interval;
                    sampleAnalysis.minInterval = interval;
                    sampleAnalysis.maxInterval = interval;
                } else {
                    sampleAnalysis.avgInterval = (sampleAnalysis.avgInterval * sampleAnalysis.numSamples + interval) / (sampleAnalysis.numSamples + 1);
                    if (interval < sampleAnalysis.minInterval) {
                        sampleAnalysis.minInterval = interval;
                    }
                    if (interval > sampleAnalysis.maxInterval) {
                        sampleAnalysis.maxInterval = interval;
                    }
                }
            }
            sampleAnalysis.numSamples++;
        }
        free(samples);
    }
    return NULL;
}