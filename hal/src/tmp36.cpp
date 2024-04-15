/****************************************************
  CITED WORK USED MAKE PANEL DISPLAY
  - https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2022-student-howtos/TMP36TemperatureSensor.pdf
  - https://www.adafruit.com/product/165
  - https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2017-student-howtos/TMP36_SensorGuide.pdf
****************************************************/

#include "hal/tmp36.h"

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095

int getVoltageReading(){
    FILE *file = fopen(A2D_FILE_VOLTAGE1, "r");
    if(!file){
        printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
        printf(" Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }

    int a2dReading = 0;
    int value = fscanf(file, "%d", &a2dReading);
    if(value <= 0){
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }

    fclose(file);
    return a2dReading;
}

int tmp36::tmp36_getCelsiusDegree(){
    int reading = getVoltageReading();
    // printf("current reading: %i\n", reading);
    
    //Conversion
    //voltage -> celsius
    double voltage = (A2D_VOLTAGE_REF_V) * ((double)reading/(double)A2D_MAX_READING);
    double temperature = (double)(1000 * voltage - 500) / (double)10;
    
    int temp = (int) temperature;

    //Cap temperature within 99
    if(temp > 99) temp = 99;
    if(temp < -99) temp = -99;

    return temp;
}