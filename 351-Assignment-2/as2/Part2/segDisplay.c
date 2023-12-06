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

#include <pthread.h>
#include <unistd.h>  // For usleep

volatile int displayThreadRunning = 0;
pthread_mutex_t mutex;
pthread_t displayThread; // Global declaration
ThreadArg* globalThreadArg = NULL;  // Global variable to hold the thread argument


DigitPattern zeroPatternR = {
    {{0x00, 0x00}, {0x02, 0x07}, {0x0e, 0x07}, {0x04, 0x05}, {0x06, 0x05}, {0x08, 0x05}, {0x0a, 0x05}, {0x0c, 0x05}}
};

DigitPattern onePatternR = {
    {{0x00, 0x00}, {0x02, 0x01}, {0x0e, 0x01}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x01}, {0x0a, 0x01}, {0x0c, 0x01}}
};

DigitPattern twoPatternR = {
    {{0x00, 0x00}, {0x02, 0x07}, {0x04, 0x04}, {0x06, 0x04}, {0x08, 0x07}, {0x0a, 0x01}, {0x0c, 0x01}, {0x0e, 0x07}}
};

DigitPattern threePatternR = {
    {{0x00, 0x00}, {0x02, 0x07}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x07}, {0x0a, 0x01}, {0x0c, 0x01}, {0x0e, 0x07}}
};

DigitPattern fourPatternR = {
    {{0x00, 0x00}, {0x02, 0x01}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x07}, {0x0a, 0x05}, {0x0c, 0x05}, {0x0e, 0x05}}
};

DigitPattern fivePatternR = {
    {{0x00, 0x00}, {0x02, 0x07}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x07}, {0x0a, 0x04}, {0x0c, 0x04}, {0x0e, 0x07}}
};

DigitPattern sixPatternR = {
    {{0x00, 0x00}, {0x02, 0x07}, {0x04, 0x05}, {0x06, 0x05}, {0x08, 0x07}, {0x0a, 0x04}, {0x0c, 0x04}, {0x0e, 0x07}}
};

DigitPattern sevenPatternR = {
    {{0x00, 0x00}, {0x02, 0x01}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x01}, {0x0a, 0x01}, {0x0c, 0x01}, {0x0e, 0x07}}
};

DigitPattern eightPatternR = {
    {{0x00, 0x00}, {0x02, 0x07}, {0x0e, 0x07}, {0x04, 0x05}, {0x06, 0x05}, {0x08, 0x07}, {0x0a, 0x05}, {0x0c, 0x05}}
};

DigitPattern ninePatternR = {
    {{0x00, 0x00}, {0x02, 0x07}, {0x04, 0x01}, {0x06, 0x01}, {0x08, 0x07}, {0x0a, 0x05}, {0x0c, 0x05}, {0x0e, 0x07}}
};

DigitPattern dotPattern = {
    {{0x00, 0x08},{0x02, 0x00}, {0x04, 0x00}, {0x06, 0x00}, {0x08, 0x00}, {0x0a, 0x00}, {0x0c, 0x00},{0x0e, 0x00}}
};

DigitPattern blankPattern = {
    {{0x00, 0x00}, {0x02, 0x00}, {0x04, 0x00}, {0x06, 0x00}, {0x08, 0x00}, {0x0a, 0x00}, {0x0c, 0x00}, {0x0e, 0x00}}
};

DigitPattern zeroPatternL = {
    {{0x00, 0x00}, {0x02, 0x07<<4}, {0x0e, 0x07<<4}, {0x04, 0x05<<4}, {0x06, 0x05<<4}, {0x08, 0x05<<4}, {0x0a, 0x05<<4}, {0x0c, 0x05<<4}}
};

DigitPattern onePatternL = {
    {{0x00, 0x00}, {0x02, 0x01<<4}, {0x0e, 0x01<<4}, {0x04, 0x01<<4}, {0x06, 0x01<<4}, {0x08, 0x01<<4}, {0x0a, 0x01<<4}, {0x0c, 0x01<<4}}
};

DigitPattern twoPatternL = {
    {{0x00, 0x00}, {0x02, 0x07<<4}, {0x04, 0x04<<4}, {0x06, 0x04<<4}, {0x08, 0x07<<4}, {0x0a, 0x01<<4}, {0x0c, 0x01<<4}, {0x0e, 0x07<<4}}
};

DigitPattern threePatternL = {
    {{0x00, 0x00}, {0x02, 0x07<<4}, {0x04, 0x01<<4}, {0x06, 0x01<<4}, {0x08, 0x07<<4}, {0x0a, 0x01<<4}, {0x0c, 0x01<<4}, {0x0e, 0x07<<4}}
};

DigitPattern fourPatternL = {
    {{0x00, 0x00}, {0x02, 0x01<<4}, {0x04, 0x01<<4}, {0x06, 0x01<<4}, {0x08, 0x07<<4}, {0x0a, 0x05<<4}, {0x0c, 0x05<<4}, {0x0e, 0x05<<4}}
};

DigitPattern fivePatternL = {
    {{0x00, 0x00}, {0x02, 0x07<<4}, {0x04, 0x01<<4}, {0x06, 0x01<<4}, {0x08, 0x07<<4}, {0x0a, 0x04<<4}, {0x0c, 0x04<<4}, {0x0e, 0x07<<4}}
};

DigitPattern sixPatternL = {
    {{0x00, 0x00}, {0x02, 0x07<<4}, {0x04, 0x05<<4}, {0x06, 0x05<<4}, {0x08, 0x07<<4}, {0x0a, 0x04<<4}, {0x0c, 0x04<<4}, {0x0e, 0x07<<4}}
};

DigitPattern sevenPatternL = {
    {{0x00, 0x00}, {0x02, 0x01<<4}, {0x04, 0x01<<4}, {0x06, 0x01<<4}, {0x08, 0x01<<4}, {0x0a, 0x01<<4}, {0x0c, 0x01<<4}, {0x0e, 0x07<<4}}
};

DigitPattern eightPatternL = {
    {{0x00, 0x00}, {0x02, 0x07<<4}, {0x0e, 0x07<<4}, {0x04, 0x05<<4}, {0x06, 0x05<<4}, {0x08, 0x07<<4}, {0x0a, 0x05<<4}, {0x0c, 0x05<<4}}
};

DigitPattern ninePatternL = {
    {{0x00, 0x00}, {0x02, 0x07<<4}, {0x04, 0x01<<4}, {0x06, 0x01<<4}, {0x08, 0x07<<4}, {0x0a, 0x05<<4}, {0x0c, 0x05<<4}, {0x0e, 0x07<<4}}
};

static void displayDigit(int i2cFileDesc, DigitPattern digit) {
    for (int i = 0; i < 8; i++) {
        writeI2cReg(i2cFileDesc, digit.patterns[i].regAddr, digit.patterns[i].value);
    }
}

static void setDisplayPatterns(double number, DigitPattern *leftDigit, DigitPattern *rightDigit, DigitPattern *dot) {
    DigitPattern digitPatternsR[10] = {zeroPatternR, onePatternR, twoPatternR, threePatternR, fourPatternR, fivePatternR, sixPatternR, sevenPatternR, eightPatternR, ninePatternR};
    DigitPattern digitPatternsL[10] = {zeroPatternL, onePatternL, twoPatternL, threePatternL, fourPatternL, fivePatternL, sixPatternL, sevenPatternL, eightPatternL, ninePatternL};
    
    if (number < 0.0) {
        fprintf(stderr, "Error: Number is less than 0\n");
        exit(EXIT_FAILURE);
    }

     // Cap for integers and doubles
    if (number > 99.0) {
        number = 99.0;
    }
    if (number > 9.9 && number != (double)((int)number)) {
        number = 9.9;
    }

    // Handling double numbers (0.0 to 9.9)
    if (number < 10.0 && number != (double)((int)number)) {
        int integerPart = (int)number;
        int decimalPart = (int)(number * 10) % 10;

        *leftDigit = digitPatternsL[integerPart];
        *rightDigit = digitPatternsR[decimalPart];
        *dot = dotPattern;
    } else {
        // Handling integer numbers (0 to 99)
        int num = (int)number;
        if (num <= 9) {
            *leftDigit = blankPattern;
            *rightDigit = digitPatternsR[num];
            *dot = blankPattern;
        } else {
            int leftNum = num / 10;
            int rightNum = num % 10;
            *leftDigit = digitPatternsL[leftNum];
            *rightDigit = digitPatternsR[rightNum];
            *dot = blankPattern;
        }
    
    }
    
    
    
}

static void* displayThreadFunc(void* arg) {
    ThreadArg* threadArg = (ThreadArg*)arg;
    double number = threadArg->number;
    int i2cFileDesc = threadArg->i2cFileDesc;

    DigitPattern leftPattern, rightPattern, dot;
    setDisplayPatterns(number, &leftPattern, &rightPattern, &dot);
    
    while (displayThreadRunning) {
      
        // Display pattern for left digit
        displayDigit(i2cFileDesc, leftPattern);

        sleepForMs(1); // 5ms delay

        // Display pattern for right digit
        displayDigit(i2cFileDesc, rightPattern); 
        
        sleepForMs(1); // 5ms delay
        displayDigit(i2cFileDesc, dot);  // Will display dot or blank
        //sleep(1);
    }

    // Turn off the display before exiting the thread
    displayDigit(i2cFileDesc, blankPattern);   
    return NULL;
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
/*
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}
    char value = 0;
    res = read(i2cFileDesc, &value, sizeof(value));
    if (res != sizeof(value)) {
        perror("I2C: Unable to read from i2c register");
        exit(1);    
    }
    return value;
}
*/

static int ledInitialize() {
    
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



void ledClose() {
    // Signal the thread to exit
    pthread_mutex_lock(&mutex);
    displayThreadRunning = 0;
    pthread_mutex_unlock(&mutex);

    // Wait for the thread to finish
    pthread_join(displayThread, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    // Free the allocated memory for thread argument
    free(globalThreadArg);
    globalThreadArg = NULL; // Set to NULL after freeing
}

void ledOpen(double numberToShow) {
    pthread_mutex_lock(&mutex);

    if (displayThreadRunning) {
        // Signal the existing thread to stop
        displayThreadRunning = 0;

        // Unlock the mutex before waiting for the thread to ensure no deadlock
        pthread_mutex_unlock(&mutex);

        // Wait for the existing thread to finish
        pthread_join(displayThread, NULL);

        // Lock the mutex again to safely update the global state
        pthread_mutex_lock(&mutex);

        // Clean up the old thread argument
        if (globalThreadArg != NULL) {
            free(globalThreadArg);
            globalThreadArg = NULL;
        }
    }

    // Initialize for the new display
    int i2cFileDesc = ledInitialize();

    globalThreadArg = malloc(sizeof(ThreadArg));
    if (globalThreadArg == NULL) {
        fprintf(stderr, "Error allocating memory for thread argument\n");
        pthread_mutex_unlock(&mutex);
        return;  // Exit the function
    }

    globalThreadArg->number = numberToShow;
    globalThreadArg->i2cFileDesc = i2cFileDesc;

    // Create a new thread for the updated display
    if (pthread_create(&displayThread, NULL, displayThreadFunc, globalThreadArg)) {
        fprintf(stderr, "Error creating display thread\n");
        free(globalThreadArg); // Free the allocated memory
        close(i2cFileDesc);    // Close the I2C file descriptor
        pthread_mutex_unlock(&mutex);
        return;  // Exit the function
    }

    displayThreadRunning = 1;
    pthread_mutex_unlock(&mutex);
}
