// Sample analysis
// Path: sample_analysis.h

#ifndef SAMPLE_ANALYSIS_H
#define SAMPLE_ANALYSIS_H

#include "sampler.h"

double getMinVoltage(void);
double getMaxVoltage(void);
double getMinInterval(void);
double getMaxInterval(void);
int getNumDips(void);

void cleanup_sample_analysis(void);
void* sampleAnalysisThreadFunc(void* arg);
void initialSampleAnalysis(void);
//printer ffuntions
void printSampleAnalysis(void);

#endif //SAMPLE_ANALYSIS_H
