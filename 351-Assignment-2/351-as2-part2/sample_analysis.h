// Sample analysis
// Path: sample_analysis.h

#ifndef SAMPLE_ANALYSIS_H
#define SAMPLE_ANALYSIS_H

// Thread initialization and cleanup functions

void cleanup_sample_analysis(void);

void* sampleAnalysisThreadFunc(void* arg);
void init_sample_analysis(void);

//printer ffuntions
void printSampleAnalysis(void);
#endif //SAMPLE_ANALYSIS_H
