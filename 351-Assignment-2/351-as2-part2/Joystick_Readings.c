#include "Joystick_Readings.h"
#include "A2D.h"
#include "common.h"
#include <stdlib.h>

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