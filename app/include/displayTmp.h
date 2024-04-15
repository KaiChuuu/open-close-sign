#ifndef _DISPLAY_TMP_H_
#define _DISPLAY_TMP_H_

#include "hal/tmp36.h"
#include "displayPanel.h"

#include <pthread.h>

class DisplayTmp {
private:

public:
    void displayTmp_init(DisplayPanel &displayPanel);
    void displayTmp_cleanup();
    void displayTmp_setTemperatureVisibility(bool display);
};

#endif