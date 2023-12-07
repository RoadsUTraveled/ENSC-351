// Description: Common functions
#ifndef COMMON_H
#define COMMON_H

void runCommand(const char *command);
void exportGPIO(const int pin);
void unexportGPIO(const int pin);
void setGPIODirection(const int pin, const char *direction);
void writeGPIO(const int pin, const int value);
int readGPIO(const int pin, int *value);

#endif // COMMON_H