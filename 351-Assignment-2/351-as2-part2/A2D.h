// ADC voltage reading functions
// Path: A2D.c
#ifndef A2D_H  
#define A2D_H 

#define ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw"

int getVoltageReading_A2D(int channel); //channel is either 2 or 3 
double getVoltage_A2D(int channel);

#endif // A2D
