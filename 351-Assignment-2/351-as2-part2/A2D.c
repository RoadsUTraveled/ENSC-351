#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "common.h"
#include "A2D.h"

static const double A2D_VOLTAGE_REF_V = 1.8;
static const int A2D_MAX_READING = 4095;

int getVoltageReading(int channel) {
    char filePath[80];
    snprintf(filePath, sizeof(filePath), ADC_PATH, channel);

    FILE *f = fopen(filePath, "r");
    if (!f) {
        printf("ERROR: Unable to open voltage input file for channel %d. Cape loaded?\n", channel);
        exit(-1);
    }

    int a2dReading = 0;
    if (fscanf(f, "%d", &a2dReading) <= 0) {
        printf("ERROR: Unable to read values from voltage input file for channel %d.\n", channel);
        fclose(f);
        exit(-1);
    }

    fclose(f);
    return a2dReading;
}

double getVoltage(int channel) {
    double a2dReading = getVoltageReading(channel);
    double voltage = a2dReading * A2D_VOLTAGE_REF_V / A2D_MAX_READING;
    return voltage;
}