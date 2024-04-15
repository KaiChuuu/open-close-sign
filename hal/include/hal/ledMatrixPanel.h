#ifndef _LED_MATRIX_PANEL_H_
#define _LED_MATRIX_PANEL_H_

#include "hal/beagle.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <iostream>

class LedMatrixPanel {
private:
    beagle board;

    void panel_exportPins();
    void panel_setPinDirection();
    void panel_openFiles();

    void panel_initializeMatrix();
    void panel_configurePins();
public:
    void panel_refreshPanel(void);

    /* 
    Can produce 8 different options
    0 : None
    1 : Red
    2 : Green
    3 : Yellow
    4 : Blue
    5 : Purple
    6 : Light blue
    7 : Even lighter blue (close to white)
    */
    //Main method to interact with panel
    void panel_setPixel(int row, int column, int color);
    void panel_printPanel();

    void panel_init(void);
    void panel_cleanup(void);
};

#endif
