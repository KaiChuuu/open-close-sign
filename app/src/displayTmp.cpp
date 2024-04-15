#include "displayTmp.h"

#define TEMPERATURE_CHECK_DELAY 1000000

static pthread_t displayTmpThread;
static bool active = true;
void* displayTmp_thread();

static bool isDisplayTemp = true;

tmp36 *temp;

void *displayTmp_thread(void *arg){
    DisplayPanel *sign = static_cast<DisplayPanel*>(arg);

    while(active){
        
        if(isDisplayTemp){
            int curTmp = temp->tmp36_getCelsiusDegree();
            sign->displayPanel_displayText(2, std::to_string(curTmp) + '*');
            // printf("current temperature: %i\n", curTmp);
        }

        usleep(TEMPERATURE_CHECK_DELAY);
    }
    return NULL;
}

void DisplayTmp::displayTmp_init(DisplayPanel &dPanel){
    pthread_create(&displayTmpThread, NULL, &displayTmp_thread, &dPanel);
}

void DisplayTmp::displayTmp_cleanup(){
    active = false;
    pthread_join(displayTmpThread, NULL);
}

void DisplayTmp::displayTmp_setTemperatureVisibility(bool display){
    isDisplayTemp = display;
}
