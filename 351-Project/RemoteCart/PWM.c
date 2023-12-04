#include "PWM.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define PWM_PERIOD 20000000

#define PWM_PATH "/sys/class/pwm/pwmchipX/" // Replace 'X' with the correct PWM chip number
#define PWM0 "pwm-0" // Replace with the correct PWM identifier for ENA
#define PWM1 "pwm-1" // Replace with the correct PWM identifier for ENB

static void writePWMFile(const char* filename, const char* value) {
    int fd = open(filename, O_WRONLY);
    if (fd == -1) {
        perror("Unable to open PWM file");
        exit(1);
    }

    if (write(fd, value, strlen(value)) != strlen(value)) {
        perror("Error writing to PWM file");
        close(fd);
        exit(1);
    }

    close(fd);
}

void PWM_Init() {
    writePWMFile(PWM_PATH PWM0 "/enable", "0");
    writePWMFile(PWM_PATH PWM1 "/enable", "0");

    writePWMFile(PWM_PATH PWM0 "/period", PWM_PERIOD);
    writePWMFile(PWM_PATH PWM1 "/period", PWM_PERIOD);

    PWM_SetDutyCycle(PWM0, "10000000");
    PWM_SetDutyCycle(PWM1, "10000000");

    writePWMFile(PWM_PATH PWM0 "/enable", "1");
    writePWMFile(PWM_PATH PWM1 "/enable", "1");
}

void PWM_SetDutyCycle(char* pwm, unsigned int dutyCycle)
{
        char path[128];
    snprintf(path, sizeof(path), "%s%s/duty_cycle", PWM_PATH, pwm);
    writePWMFile(path, dutyCycle);
}

