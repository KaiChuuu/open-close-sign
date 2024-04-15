#include "controller.h"
#include "displayPanel.h"
#include "network.h"
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <thread>

static bool isRunning = false;
static std::thread controllerThread;
static void controllerLoop(DisplayPanel &displayPanel);

void Controller::init(DisplayPanel &displayPanel) {

    if (isRunning) {
        return;
    }
    isRunning = true;
    controllerThread = std::thread(controllerLoop, std::ref(displayPanel));
}

void Controller::cleanup() {
    if (!isRunning) {
        return;
    }
    isRunning = false;
    controllerThread.join();
}

void controllerLoop(DisplayPanel &displayPanel) {
    Network network;
    while (isRunning) {
        std::string msg = network.receiveData();
        std::size_t pos = msg.find(":");
        std::cout << "Message received: " << msg << std::endl;
        if (msg.substr(0, pos) == "UPDATE") {
            displayPanel.displayPanel_displayText(0, msg.substr(pos+1), 1, 2);
            std::cout << "Message written to display: " << msg.substr(pos+1) << std::endl;
            displayPanel.displayPanel_printPanel();
        }
        if(msg.substr(0, pos) == "SCHED"){
            displayPanel.displayPanel_displayText(1, msg.substr(pos+1,msg.length()-1-pos), 0, 2);
            std::cout << "Schedule updated in display: " << msg.substr(pos+1) << std::endl;
            displayPanel.displayPanel_printPanel();
        }
        if (msg.substr(0, pos) == "EXIT") {
            //end program
        }
    }
}
