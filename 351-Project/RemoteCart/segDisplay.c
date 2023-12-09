#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "common.h"
#include "segDisplay.h"
#include <stdbool.h>


#include <unistd.h>  // For usleep



DigitPattern blankPattern = {
    {{0x00, 0x00}, {0x02, 0x00}, {0x04, 0x00}, {0x06, 0x00}, {0x08, 0x00}, {0x0a, 0x00}, {0x0c, 0x00}, {0x0e, 0x00}}
};

DigitPattern rightPattern = {
    {{0x00, 0x00}, {0x02, 0x00}, {0x04, 0x04}, {0x06, 0x02}, {0x08, 0x3F}, {0x0a, 0x02}, {0x0c, 0x04}, {0x0e, 0x00}}
};

DigitPattern leftPattern = {
    {{0x00, 0x00}, {0x02, 0x00}, {0x04, 0x08}, {0x06, 0x10}, {0x08, 0x3F}, {0x0a, 0x10}, {0x0c, 0x08}, {0x0e, 0x00}}
};

DigitPattern upPattern = {
    {{0x00, 0x00}, {0x02, 0x08}, {0x04, 0x08}, {0x06, 0x08}, {0x08, 0x2A}, {0x0a, 0x1C}, {0x0c, 0x08}, {0x0e, 0x00}}
};

DigitPattern downPattern = {
    {{0x00, 0x00}, {0x02, 0x08}, {0x04, 0x1C}, {0x06, 0x2A}, {0x08, 0x08}, {0x0a, 0x08}, {0x0c, 0x08}, {0x0e, 0x00}}
};

static void displayDigit(int i2cFileDesc, DigitPattern digit) {
    for (int i = 0; i < 8; i++) {
        writeI2cReg(i2cFileDesc, digit.patterns[i].regAddr, digit.patterns[i].value);
    }
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

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
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


int ledInitialize() {
    
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");

    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    // Set direction of I2C GPIO extender ports to be outputs
    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);

    // Drive an hour-glass looking character (Like an X with a bar on top & bottom)
    writeI2cReg(i2cFileDesc, REG_OUTA, 0x2A);
    writeI2cReg(i2cFileDesc, REG_OUTB, 0x54);

    // Read and print a register for verification
    //unsigned char regVal = readI2cReg(i2cFileDesc, REG_OUTA);
    //printf("Reg OUT-A = 0x%02x\n", regVal);

    // Additional I2C register configurations
    writeI2cReg(i2cFileDesc, 0x21, 0x00);
    writeI2cReg(i2cFileDesc, 0x81, 0x00);
    
    writeI2cReg(i2cFileDesc, 0x00, 0x00);
    writeI2cReg(i2cFileDesc, 0x02, 0x00);
    writeI2cReg(i2cFileDesc, 0x04, 0x00);
    writeI2cReg(i2cFileDesc, 0x06, 0x00);
    writeI2cReg(i2cFileDesc, 0x08, 0x00);
    writeI2cReg(i2cFileDesc, 0x0a, 0x00);
    writeI2cReg(i2cFileDesc, 0x0c, 0x00);
    writeI2cReg(i2cFileDesc, 0x0e, 0x00);
    
    return i2cFileDesc;
}

void displayPatternBasedOnInput(int i2cFileDesc, int input1, int input2) {
    if (input1 == 0 && input2 == 0) {
        displayDigit(i2cFileDesc, downPattern);
    } else if (input1 == 1 && input2 == 1) {
        displayDigit(i2cFileDesc, upPattern);
    } else if (input1 == 1 && input2 == 0) {
        displayDigit(i2cFileDesc, leftPattern);
    } else if (input1 == 0 && input2 == 1) {
        displayDigit(i2cFileDesc, rightPattern);
    } else {
        displayDigit(i2cFileDesc, blankPattern);
    }
}


void cleanLed(int i2cFileDesc){
    displayDigit(i2cFileDesc, blankPattern);
}
