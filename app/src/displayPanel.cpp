#include "displayPanel.h"
#include "displayFont.h"
#include "layer.h"
#include <array>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

static pthread_t displayPanelThread;
static pthread_t refreshPanelThread;
static bool active = true;

static std::vector<Layer> layers;

// 2D array that we apply all the layers to before applying
// the whole thing to the actual sign
static std::mutex bufferMutex;
static std::array<std::array<int, 32>, 16> displayBuffer {{{ 0 }}};
static std::array<DisplayFont, 2> fonts;

static void displayPanel_loadLayersIntoBuffer();
static void displayPanel_loadBufferIntoSign(LedMatrixPanel *signRef);

DisplayPanel::DisplayPanel(LedMatrixPanel *signRef){
    sign = signRef;
}

void *displayPanel_thread(void* arg){
    LedMatrixPanel *sign = static_cast<LedMatrixPanel*>(arg);

    while(active){
        displayPanel_loadLayersIntoBuffer();
        displayPanel_loadBufferIntoSign(sign);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //change for speed
    }
    return NULL;
}

void *refreshPanel_thread(void *arg){
    LedMatrixPanel *sign = static_cast<LedMatrixPanel*>(arg);
    while(active){
        sign->panel_refreshPanel();
    }
    return NULL;
}

void DisplayPanel::displayPanel_init(){
    fonts[0].font_LoadFont("./fonts/5x3");
    fonts[1].font_LoadFont("./fonts/7x5");

    displayPanel_loadLayersIntoBuffer();
    // For now, hard code layer sizes
    layers.emplace(layers.end(), 32, 9, 0, 0);
    layers.emplace(layers.end(), 22, 6, 10, 0);
    layers.emplace(layers.end(), 9, 6, 10, 23);

    // Draw borders
    layers.emplace(layers.end(), 32, 1, 9, 0);
    layers.emplace(layers.end(), 1, 9, 9, 21);

    pthread_create(&displayPanelThread, NULL, &displayPanel_thread, &sign);
    pthread_create(&refreshPanelThread, NULL, &refreshPanel_thread, &sign);
}

void DisplayPanel::displayPanel_cleanup(){
    active = false;
    pthread_join(displayPanelThread, NULL);
    pthread_join(refreshPanelThread, NULL);
}

void DisplayPanel::displayPanel_clearDisplay() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 32; j++) {
            sign->panel_setPixel(i, j, 0);
        }
    }
}

void DisplayPanel::displayPanel_displayText(int layer, std::string text, int size, int color, int backgroundColor) {
    if (layer < 0 || (size_t)layer >= layers.size()) {
        return;
    }
    std::lock_guard<std::mutex> lockGuard(bufferMutex);
    layers[layer].placeTextInLayer(text, fonts[size], color, backgroundColor);
}

void DisplayPanel::displayPanel_displayText(int layer, std::string text, std::vector<int> &colorSequence,
        int size, int backgroundColor) {
    if (layer < 0 || (size_t)layer >= layers.size() || colorSequence.size() == 0) {
        return;
    }
    std::lock_guard<std::mutex> lockGuard(bufferMutex);
    layers[0].placeTextInLayer(text, fonts[size], colorSequence, backgroundColor);

}

void DisplayPanel::displayPanel_fillWithColor(int layer, int color, bool animate) {
    layers[layer].fillWithColor(color, animate);
}

void DisplayPanel::displayPanel_fillWithColor(int layer, std::vector<int> &colorSequence, bool animate) {
    layers[layer].fillWithColor(colorSequence, animate);
}

static void displayPanel_loadLayersIntoBuffer() {
    for (Layer &layer : layers) {
        const std::vector<std::vector<int>> &matrix = layer.getLayerGrid();
        const size_t width = matrix[0].size();
        for (int i = 0; i < layer.height; i++) {
            for (int j = 0; j < layer.width; j++) {
                displayBuffer[i + layer.row_offset][j + layer.col_offset] = matrix[i][(layer.scrollOffset + j) % width];
            }
        }
        layer.updateLayer();
    }
}

static void displayPanel_loadBufferIntoSign(LedMatrixPanel *signRef) {
    std::lock_guard<std::mutex> lockGuard(bufferMutex);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 32; j++) {
            signRef->panel_setPixel(i, j, displayBuffer[i][j]);
        }
    }
}

void DisplayPanel::displayPanel_printPanel() {
    sign->panel_printPanel();
}
