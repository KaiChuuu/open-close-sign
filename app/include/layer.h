
#ifndef _LAYER_H_
#define _LAYER_H_

#include "displayFont.h"
#include <cstdint>
#include <string>
#include <vector>
class Layer {
private:
    std::vector<std::vector<int>> layerGrid;

    void layer_clearLayer();
    void layer_placeChar(int row_offset, int col_offset, char c, int color, int backgroundColor, DisplayFont &font);
public:
    bool enabled;
    int width;
    int height;
    int row_offset;
    int col_offset;
    uint32_t scrollDuration;
    uint32_t scrollOffset;

    Layer(int width, int height, int row_offset, int col_offset);
    ~Layer();

    // Place words in the layer, all in the same color
    void placeTextInLayer(const std::string &text,
            DisplayFont &font,
            const int color=1,
            const int backgroundColor=0,
            int textRowOffset=1, int textColOffset=0);

    void placeTextInLayer(const std::string &text,
            DisplayFont &font,
            std::vector<int> &colorSequence,
            const int backgroundColor=0,
            int textRowOffset=1, int textColOffset=0);

    // Given a color, it will draw the border around the layer
    void drawBorder(int borderColor = 1);

    void updateLayer();

    // Given a vector of colors, it will draw the border in the following sequence
    // for each pixel
    void drawBorder(const std::vector<int> &colorSequence);

    void fillWithColor(int color, bool animate=false);
    void fillWithColor(std::vector<int> &colorSequence, bool animate=false);

    const std::vector<std::vector<int>>& getLayerGrid();
};

#endif
