#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

// Path: common.h

void exportGPIO(int gpio)
{
    int fd = open(GPIO_EXPORT, O_WRONLY);
    if (fd == -1)
    {
        perror("Unable to open /sys/class/gpio/export");
        exit(1);
    }

    char buffer[3];
    sprintf(buffer, "%d", gpio);
    if (write(fd, buffer, strlen(buffer)) != strlen(buffer))
    {
        perror("Error writing to /sys/class/gpio/export");
        exit(1);
    }

    close(fd);
}

void unexportGPIO(int gpio)
{
    int fd = open(GPIO_UNEXPORT, O_WRONLY);
    if (fd == -1)
    {
        perror("Unable to open /sys/class/gpio/unexport");
        exit(1);
    }

    char buffer[3];
    sprintf(buffer, "%d", gpio);
    if (write(fd, buffer, strlen(buffer)) != strlen(buffer))
    {
        perror("Error writing to /sys/class/gpio/unexport");
        exit(1);
    }

    close(fd);
}

void setGPIO_Direction(int gpio, char *direction)
{
    char path[35];
    sprintf(path, GPIO_DIRECTION, gpio);

    int fd = open(path, O_WRONLY);
    if (fd == -1)
    {
        perror("Unable to open GPIO direction file");
        exit(1);
    }

    if (write(fd, direction, strlen(direction)) != strlen(direction))
    {
        perror("Error writing to GPIO direction file");
        exit(1);
    }

    close(fd);
}

void setGPIO_Value(int gpio, int value)
{
    char path[30];
    sprintf(path, GPIO_VALUE, gpio);

    int fd = open(path, O_WRONLY);
    if (fd == -1)
    {
        perror("Unable to open GPIO value file");
        exit(1);
    }

    char buffer[2];
    sprintf(buffer, "%d", value);
    if (write(fd, buffer, 1) != 1)
    {
        perror("Error writing to GPIO value file");
        exit(1);
    }

    close(fd);
}