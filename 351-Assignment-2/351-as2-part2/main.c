#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Include all the header files
#include "common.h"
#include "computing_dips.h"
#include "display.h"
#include "Joystick_Readings.h"
#include "sample_analysis.h"
#include "sample_printer.h"
#include "sampler.h"
#include "user_button_exit.h"
#include "A2D.h"

int main() {
    // test the A2D functions
    while (true) {
        int reading = getVoltageReading(2);
        double voltage = getVoltage(2);
        printf("Reading: %d, Voltage: %f\n", reading, voltage);
    }

    return 0;
}