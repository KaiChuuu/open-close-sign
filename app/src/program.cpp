#include "program.h"
#include <chrono>
#include <thread>
#include <vector>

Program::Program():
displayPanel(&ledMatrixPanel)
{

}

//ORDER MATTERS
void Program::program_init(){
    //hal
    ledMatrixPanel.panel_init();

    //app
    displayPanel.displayPanel_init();


    displayPanel.displayPanel_displayText(0, "THIS IS A LONG MESSAGE THAT MAY LOOP", 1, 1, 4);
    displayPanel.displayPanel_displayText(1, "T09:30");
    displayPanel.displayPanel_displayText(2, "23");

    std::vector<int> colorSequence = { 1, 2 };
    displayPanel.displayPanel_displayText(0, "HI THERE", colorSequence, 1);

    displayPanel.displayPanel_fillWithColor(3, colorSequence, true);
    displayPanel.displayPanel_fillWithColor(4, colorSequence, true);

    displayTmp.displayTmp_init(displayPanel);

    Controller::init(displayPanel);
}

//ORDER MATTERS
void Program::program_cleanup(){
    Controller::cleanup();
    //app
    displayTmp.displayTmp_cleanup();
    displayPanel.displayPanel_cleanup();

    //hal
    ledMatrixPanel.panel_cleanup();
}
