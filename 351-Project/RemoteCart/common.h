// Description: Common functions
// exportGPIO() - exports a GPIO
// unexportGPIO() - unexports a GPIO
// setGPIO_Direction() - sets the direction of a GPIO
// setGPIO_Value() - sets the value of a GPIO
#ifndef COMMON_H
#define COMMON_H

#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_UNEXPORT "/sys/class/gpio/unexport"
#define GPIO_DIRECTION "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE "/sys/class/gpio/gpio%d/value"

void exportGPIO(int gpio);
void unexportGPIO(int gpio);
void setGPIO_Direction(int gpio, char* direction);
void setGPIO_Value(int gpio, int value);

#endif // COMMON_H