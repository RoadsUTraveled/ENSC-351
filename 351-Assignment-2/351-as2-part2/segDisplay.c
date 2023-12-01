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

void displayDigit(int i2cFileDesc, DigitPattern digit) {
    for (int i = 0; i < 8; i++) {
        writeI2cReg(i2cFileDesc, digit.patterns[i].regAddr, digit.patterns[i].value);
    }
}

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
}
void setDisplayPatterns(double number, DigitPattern *leftDigit, DigitPattern *rightDigit, DigitPattern *dot) {
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

void* displayThreadFunc(void* arg) {
    ThreadArg* threadArg = (ThreadArg*)arg;
    double number = threadArg->number;
    int i2cFileDesc = threadArg->i2cFileDesc;

    DigitPattern leftPattern, rightPattern, dot;
    setDisplayPatterns(number, &leftPattern, &rightPattern, &dot);
    
    while (displayThreadRunning) {
        // Turn off both digits
        gpioWrite(61, 0);
        gpioWrite(44, 0);
        
        // Display pattern for left digit
        displayDigit(i2cFileDesc, leftPattern);

        // Turn on left digit
        gpioWrite(61, 1);
        sleepForMs(5); // 5ms delay

        // Turn off both digits again
        gpioWrite(61, 0);
        gpioWrite(44, 0);

        // Display pattern for right digit
        displayDigit(i2cFileDesc, rightPattern); 

        // Turn on right digit
        gpioWrite(44, 1);
        
        sleepForMs(5); // 5ms delay
        displayDigit(i2cFileDesc, dot);  // Will display dot or blank
    }

    // Turn off the display before exiting the thread
    displayDigit(i2cFileDesc, blankPattern);
    gpioWrite(61, 0);
    gpioWrite(44, 0);
    runCommand("echo 61 > /sys/class/gpio/unexport");
    runCommand("echo 44 > /sys/class/gpio/unexport");
    return NULL;
}

int initI2cBus(char* bus, int address)
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


/*
int ledOpen(double numberToShow) {
        // LED initialization
    int i2cFileDesc = ledInitialize();

    // Initialize the thread argument
    ThreadArg threadArg;
    threadArg.number = numberToShow;  
    threadArg.i2cFileDesc = i2cFileDesc;

    // Create the display thread
    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&displayThread, NULL, displayThreadFunc, &threadArg)) {
        fprintf(stderr, "Error creating display thread\n");
        return -1;  // Return an error code
    }
 
    pthread_join(displayThread, NULL);
    return i2cFileDesc;
}


void ledClose(){
    //sleep(5);
    // Signal the thread to exit
    pthread_mutex_lock(&mutex);
    displayThreadRunning = 0;
    pthread_mutex_unlock(&mutex);

    // Wait for the thread to finish
    //pthread_join(displayThread, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
}

	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}
*/
int ledInitialize() {
    // Configure I2C and GPIO pins
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");
    
    runCommand("config-pin p8.26 gpio");
    runCommand("config-pin p8.12 gpio");

    // Export GPIO pins and set direction
    runCommand("cd /sys/class/gpio");
    sleep(1);
    runCommand("echo 61 > /sys/class/gpio/export");
    sleep(1);
    runCommand("echo out > /sys/class/gpio/gpio61/direction");
    sleep(1);
    runCommand("echo 44 > /sys/class/gpio/export");
    sleep(1);
    runCommand("echo out > /sys/class/gpio/gpio44/direction");

    printf("Drive display (assumes GPIO #61 and #44 are output and 1)\n");

    // Initialize the I2C bus
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

    // Set direction of I2C GPIO extender ports to be outputs
    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);

    // Drive an hour-glass looking character (Like an X with a bar on top & bottom)
    writeI2cReg(i2cFileDesc, REG_OUTA, 0x2A);
    writeI2cReg(i2cFileDesc, REG_OUTB, 0x54);

    // Read and print a register for verification
    unsigned char regVal = readI2cReg(i2cFileDesc, REG_OUTA);
    printf("Reg OUT-A = 0x%02x\n", regVal);

    // Additional I2C register configurations
    writeI2cReg(i2cFileDesc, 0x21, 0x00);
    writeI2cReg(i2cFileDesc, 0x81, 0x00);

    return i2cFileDesc;
}

/*
int ledOpen(double numberToShow) {
        // LED initialization
    int i2cFileDesc = ledInitialize();

    // Initialize the thread argument
    ThreadArg threadArg;
    threadArg.number = numberToShow;  
    threadArg.i2cFileDesc = i2cFileDesc;

    // Create the display thread
    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&displayThread, NULL, displayThreadFunc, &threadArg)) {
        fprintf(stderr, "Error creating display thread\n");
        return -1;  // Return an error code
    }
 
    pthread_join(displayThread, NULL);
    return i2cFileDesc;
}


void ledClose(){
    //sleep(5);
    // Signal the thread to exit
    pthread_mutex_lock(&mutex);
    displayThreadRunning = 0;
    pthread_mutex_unlock(&mutex);

    // Wait for the thread to finish
    //pthread_join(displayThread, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
}
*/
int ledOpen(double numberToShow) {
    displayThreadRunning = 1;
    // LED initialization
    int i2cFileDesc = ledInitialize();

    // Allocate memory for the thread argument
    globalThreadArg = malloc(sizeof(ThreadArg));
    if (globalThreadArg == NULL) {
        fprintf(stderr, "Error allocating memory for thread argument\n");
        return -1;  // Return an error code
    }
    globalThreadArg->number = numberToShow;
    globalThreadArg->i2cFileDesc = i2cFileDesc;

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create the display thread
    if (pthread_create(&displayThread, NULL, displayThreadFunc, globalThreadArg)) {
        fprintf(stderr, "Error creating display thread\n");
        free(globalThreadArg); // Free the allocated memory
        return -1;  // Return an error code
    }

    // Return the I2C file descriptor
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


