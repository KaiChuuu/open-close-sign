#ifndef _DISPLAY_PANEL_H_
#define _DISPLAY_PANEL_H_

#include "displayFont.h"
#include "hal/ledMatrixPanel.h"

#include <pthread.h>
#include <unistd.h>
#include <vector>

class DisplayPanel {
private:
    LedMatrixPanel *sign;
    DisplayFont font;

    void displayPanel_clearDisplay();
    void displayPanel_displayChar(int row_offset, int col_offset, char c, int color);
public:
    DisplayPanel(LedMatrixPanel *signRef);
    void displayPanel_init(void);
    void displayPanel_cleanup(void);
    void displayPanel_displayText(int layer, std::string text, int size=0, int color=1, int backgroundColor=0);
    void displayPanel_displayText(int layer, std::string text, std::vector<int> &colorSequence, int size=0, int backgroundColor=0);
    void displayPanel_fillWithColor(int layer, int color, bool animate=false);
    void displayPanel_fillWithColor(int layer, std::vector<int> &colorSequence, bool animate=false);
    void displayPanel_printPanel();
};

#endif
