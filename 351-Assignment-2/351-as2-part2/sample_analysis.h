// Sample analysis
// Path: sample_analysis.h

#ifndef SAMPLE_ANALYSIS_H
#define SAMPLE_ANALYSIS_H

// Thread initialization and cleanup functions
void init_sample_analysis(void);
void cleanup_sample_analysis(void);

// Sample analysis functions
double getAvgVoltage(void);
double getMinVoltage(void);
double getMaxVoltage(void);
double getAvgInterval(void);
double getMinInterval(void);
double getMaxInterval(void);
int getNumSamples(void);
int getNumDips(void);

//printer ffuntions
void printSampleAnalysis(void);
#endif //SAMPLE_ANALYSIS_H