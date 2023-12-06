#include "Joystick_Readings.h"
#include "A2D.h"
#include "sample_analysis.h"
#include "segDisplay.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

void Joystick_readXY(double *x, double *y)
{
    if (x == NULL || y == NULL)
    {
        printf("Error: NULL pointer passed to Joystick_readXY\n");
        exit(1);
    }

    int rawX = getVoltageReading(2), rawY = getVoltageReading(3);

    *x = (double)rawX / 4095.0 * 2.0 - 1.0;
    *y = -((double)rawY / 4095.0 * 2.0 - 1.0);
    return;
}

void joystick_display(double x, double y)
{   
    
        if (x < -0.5) {
            double mininterval = getMinInterval();
            ledOpen(mininterval);
            //printf("Min interval: %f\n",mininterval);
        } else if (x > 0.5){
            double maxinterval = getMaxInterval();
            ledOpen(maxinterval);
            //printf("Max interval: %f\n",maxinterval);
        } else if (y > 0.5) {
            double minvoltage = getMinVoltage();
            ledOpen(minvoltage);
            //printf("Min Vol: %f\n",minvoltage);
        } else if (y < -0.5) {
            double maxvoltage = getMaxVoltage();
            ledOpen(maxvoltage);
            //printf("Max Vol: %f\n",maxvoltage);
        }
        else {
            int numDips = getNumDips();
            ledOpen(numDips);
            //printf("Dips: %d\n",numDips);
        }
        //sleep(1);
    
    
    return; 
}
