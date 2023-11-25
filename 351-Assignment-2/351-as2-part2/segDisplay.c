#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "common.h"

#include <pthread.h>
#include <unistd.h>  // For usleep

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x70

#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

static int initI2cBus(char* bus, int address);
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);

typedef struct {
    unsigned char regAddr;
    unsigned char value;
} SegmentPattern;

typedef struct {
    SegmentPattern patterns[8];
} DigitPattern;
DigitPattern zeroPattern = {
    {{0x02, 0x07}, {0x0e, 0x07}, {0x04, 0x05}, {0x06, 0x05}, {0x08, 0x05}, {0x0a, 0x05}, {0x0c, 0x05}}
};

DigitPattern onePattern = {
    {{0x02, 0x01}, {0x0e, 0x01}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x01}, {0x0a, 0x01}, {0x0c, 0x01}}
};

DigitPattern twoPattern = {
    {{0x02, 0x07}, {0x04, 0x04}, {0x06, 0x04}, {0x08, 0x07}, {0x0a, 0x01}, {0x0c, 0x01}, {0x0e, 0x07}}
};

DigitPattern threePattern = {
    {{0x02, 0x07}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x07}, {0x0a, 0x01}, {0x0c, 0x01}, {0x0e, 0x07}}
};

DigitPattern fourPattern = {
    {{0x02, 0x01}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x07}, {0x0a, 0x05}, {0x0c, 0x05}, {0x0e, 0x05}}
};

DigitPattern fivePattern = {
    {{0x02, 0x07}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x07}, {0x0a, 0x04}, {0x0c, 0x04}, {0x0e, 0x07}}
};

DigitPattern sixPattern = {
    {{0x02, 0x07}, {0x04, 0x05}, {0x06, 0x05}, {0x08, 0x07}, {0x0a, 0x04}, {0x0c, 0x04}, {0x0e, 0x07}}
};

DigitPattern sevenPattern = {
    {{0x02, 0x01}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x01}, {0x0a, 0x01}, {0x0c, 0x01}, {0x0e, 0x07}}
};

DigitPattern eightPattern = {
    {{0x02, 0x07}, {0x0e, 0x07}, {0x04, 0x05}, {0x06, 0x05}, {0x08, 0x07}, {0x0a, 0x05}, {0x0c, 0x05}}
};

DigitPattern ninePattern = {
    {{0x02, 0x07}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x07}, {0x0a, 0x05}, {0x0c, 0x05}, {0x0e, 0x07}}
};

DigitPattern dotPattern = {
    {{0x00, 0x08}, {0x02, 0x00}, {0x04, 0x00}, {0x06, 0x00}, {0x08, 0x00}, {0x0a, 0x00}, {0x0c, 0x0}, {0x0e, 0x00}}
};
void displayDigit(int i2cFileDesc, DigitPattern digit) {
    for (int i = 0; i < 7; i++) {
        writeI2cReg(i2cFileDesc, digit.patterns[i].regAddr, digit.patterns[i].value);
    }
}
/*
void gpioWrite(int gpio, int value) {
    char path[35];
    int fd;

    // Construct the path to the value file of the GPIO pin
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);

    // Open the file for writing
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Unable to open GPIO value file");
        return;
    }

    // Write the value to the file
    if (value == 0) {
        if (write(fd, "0", 1) != 1) {
            perror("Error writing to GPIO value file");
        }
    } else {
        if (write(fd, "1", 1) != 1) {
            perror("Error writing to GPIO value file");
        }
    }

    // Close the file
    close(fd);
}*/
/*
void* displayThreadFunc(void* arg) {
    int i2cFileDesc = *((int*)arg); // Cast and dereference the argument to get the I2C file descriptor

    while (1) {
        // Turn off both digits
        gpioWrite(61, 0);
        gpioWrite(44, 0);

        // Display pattern for left digit
        displayDigit(i2cFileDesc, onePattern);

        // Turn on left digit
        gpioWrite(61, 1);
        sleep(5000); // 5ms delay

        // Turn off both digits again
        gpioWrite(61, 0);
        gpioWrite(44, 0);

        // Display pattern for right digit
        displayDigit(i2cFileDesc, eightPattern); 

        // Turn on right digit
        gpioWrite(44, 1);
        sleep(5000); // 5ms delay
    }
    return NULL;
}
*/

int main()
{
 
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");
    /*
    runCommand("config-pin p8.26 gpio");
    runCommand("config-pin p8.12 gpio");
    runCommand("cd /sys/class/gpio");
    runCommand("echo 61 > /sys/class/gpio/export");
    runCommand("echo 44 > /sys/class/gpio/export");
    runCommand("echo out > /sys/class/gpio/gpio61/direction");
    runCommand("echo out > /sys/class/gpio/gpio44/direction");*/

	printf("Drive display (assumes GPIO #61 and #44 are output and 1\n");
	int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    
	writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
	writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);

	// Drive an hour-glass looking character (Like an X with a bar on top & bottom)
	writeI2cReg(i2cFileDesc, REG_OUTA, 0x2A);
	writeI2cReg(i2cFileDesc, REG_OUTB, 0x54);
    

	// Read a register:
	unsigned char regVal = readI2cReg(i2cFileDesc, REG_OUTA);
	printf("Reg OUT-A = 0x%02x\n", regVal);

    writeI2cReg(i2cFileDesc, 0x21, 0x00);
    writeI2cReg(i2cFileDesc, 0x81, 0x00);

    //test number 
    displayDigit(i2cFileDesc,sixPattern);

    /*
    writeI2cReg(i2cFileDesc, 0x00, 0x00);
    writeI2cReg(i2cFileDesc, 0x02, 0x00);
    writeI2cReg(i2cFileDesc, 0x04, 0x00);
    writeI2cReg(i2cFileDesc, 0x06, 0x00);
    writeI2cReg(i2cFileDesc, 0x08, 0x00);
    writeI2cReg(i2cFileDesc, 0x0a, 0x00);
    writeI2cReg(i2cFileDesc, 0x0c, 0x00);
    writeI2cReg(i2cFileDesc, 0x0e, 0x00);
    */

  
    //pthread_t displayThread;
    //pthread_create(&displayThread, NULL, displayThreadFunc, &i2cFileDesc);
    
    //thread_join(displayThread, NULL);
    
    // Cleanup I2C access;
	close(i2cFileDesc);
	return 0;
}

static int initI2cBus(char* bus, int address)
{
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("Unable to write i2c register");
		exit(-1);
	}
}

static unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}



