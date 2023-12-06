// Description: Common functions
// exportGPIO() - exports a GPIO
// unexportGPIO() - unexports a GPIO
// setGPIO_Direction() - sets the direction of a GPIO
// setGPIO_Value() - sets the value of a GPIO
#ifndef COMMON_H
#define COMMON_H

void runCommand(const char *command);
void exportGPIO(const int pin);
void unexportGPIO(const int pin);
void setGPIODirection(const int pin, const char *direction);
void writeGPIO(const int pin, const int value);

#endif // COMMON_H