#ifndef _BEAGLE_H_
#define _BEAGLE_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string>

class beagle {
public:
    void beagle_updateFile(const char* fileName, const char* status);

    void beagle_exportAndOut(int pinNum);
};

#endif