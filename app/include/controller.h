
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "displayPanel.h"
#include <thread>

namespace Controller {
    void init(DisplayPanel &displayPanel);
    void cleanup();
}

#endif
