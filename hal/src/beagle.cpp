#include "hal/beagle.h"

#define GPIO_PATH "/sys/class/gpio/"

void beagle::beagle_updateFile(const char* fileName, const char* status){
    printf("%s %s\n", fileName, status);
    FILE *pFile = fopen(fileName, "w");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    fprintf(pFile, "%s", status);
    // Close
    fclose(pFile);
}