// Sample analysis
// Path: sample_analysis.h

#ifndef SAMPLE_ANALYSIS_H
#define SAMPLE_ANALYSIS_H

#include "sampler.h"

// Thread initialization and cleanup functions

void cleanup_sample_analysis(void);

void* sampleAnalysisThreadFunc(void* arg);
void resetSampleAnalysis(void);

void computeStatistics(samplerDatapoint_t* samples, int length);

//printer ffuntions
void printSampleAnalysis(void);
#endif //SAMPLE_ANALYSIS_H
