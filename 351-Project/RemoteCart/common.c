#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// Path: common.h

// run a Linux command from in your C program
void runCommand(const char *command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0)
    {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

void exportGPIO(const int pin) {
    char command[50];
    sprintf(command, "echo %d > /sys/class/gpio/export", pin);
    runCommand(command);
}

void unexportGPIO(const int pin) {
    char command[50];
    sprintf(command, "echo %d > /sys/class/gpio/unexport", pin);
    runCommand(command);
}

void setGPIODirection(const int pin, const char *direction) {
    char command[80];
    sprintf(command, "echo %s > /sys/class/gpio/gpio%d/direction", direction, pin);
    runCommand(command);
}

void writeGPIO(const int pin, const int value) {
    char command[80];
    sprintf(command, "echo %d > /sys/class/gpio/gpio%d/value", value, pin);
    runCommand(command);
}

int readGPIO(const int pin, int *value) {
    char path[50];
    sprintf(path, "/sys/class/gpio/gpio%d/value", pin);
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error opening GPIO value file");
        return -1;
    }

    char valueStr[3];
    fgets(valueStr, sizeof(valueStr), fp);
    fclose(fp);

    *value = atoi(valueStr);
    return 0;
}