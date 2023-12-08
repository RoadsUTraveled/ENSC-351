#ifndef SEGDISPLAY_H
#define SEGDISPLAY_H

//#include <linux/i2c.h>
//#include <linux/i2c-dev.h>

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x70

#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15


typedef struct {
    unsigned char regAddr;
    unsigned char value;
} SegmentPattern;

typedef struct {
    SegmentPattern patterns[8];
} DigitPattern;

typedef struct {
    double number;
    int i2cFileDesc;
} ThreadArg;

// Function prototypes
//int initI2cBus(char* bus, int address);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
//unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);
//void displayDigit(int i2cFileDesc, DigitPattern digit);
//void setDisplayPatterns(double number, DigitPattern *leftDigit, DigitPattern *rightDigit, DigitPattern*dot);
//void* displayThreadFunc(void* arg);
//int ledInitialize();
void cleanLed(int i2cFileDesc);
void displayPatternBasedOnInput(int i2cFileDesc, int input1, int input2);
int ledInitialize();



#endif // SEGDISPLAY_H