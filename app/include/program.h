#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "displayPanel.h"
#include "displayTmp.h"
#include "controller.h"
#include "hal/ledMatrixPanel.h"

class Program {
private:
    LedMatrixPanel ledMatrixPanel;
    DisplayPanel displayPanel;
    DisplayTmp displayTmp;
public:
    Program();
    void program_init(void);
    void program_cleanup(void);
};

#endif