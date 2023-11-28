#ifndef USERBUTTON_H
#define USERBUTTON_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h> // For bool, true, false
#include <unistd.h>  // For usle
#include <stdbool.h>

#define BUTTON "/sys/class/gpio/gpio72/value"
int readButton(void);
void* buttonMonitorThreadFunc(void* arg);
void controlButtonMonitorThread();

#endif