#include "layer.h"
#include "displayFont.h"
#include <cstdint>
#include <iostream>
#include <vector>

Layer::Layer(int width, int height, int row_offset, int col_offset) :
    layerGrid(),
    enabled(true), width(width), height(height), 
    row_offset(row_offset), col_offset(col_offset),
    scrollDuration(5000), scrollOffset(0)
{
    layerGrid.resize(height);
    for (int i = 0; i < height; i++) {
        layerGrid[i].resize(width);
    }
    std::cout << "Constructed: " << layerGrid.size() << " " << layerGrid[0].size() << std::endl;
}

Layer::~Layer() {}

void Layer::layer_clearLayer() {
    for (size_t i = 0; i < layerGrid.size(); i++) {
        for (size_t j = 0; j < layerGrid[0].size(); j++) {
            layerGrid[i][j] = 0;
        }
    }
}

void Layer::layer_placeChar(int row_offset, int col_offset, char c, int color, int backgroundColor, DisplayFont &font) {
    const CharMatrix &charMatrix = font.font_GetCharMatrix(c);
    if (charMatrix.height == 0 || charMatrix.width == 0) {
        return;
    }
    for (int i = 0; i < charMatrix.height; i++) {
        for (int j = 0; j < charMatrix.width; j++) {
            if (charMatrix.matrix[i][j]) {
                layerGrid[row_offset + i][col_offset + j] = color;
            } else {
                layerGrid[row_offset + i][col_offset + j] = backgroundColor;
            }
        }
    }
}

void Layer::placeTextInLayer(const std::string &text,
        DisplayFont &font,
        const int color,
        const int backgroundColor,
        int textRowOffset, int textColOffset) {
    scrollOffset = 0;
    layer_clearLayer();
    int totalWidth = 0;
    int totalHeight = height;
    
    // Calculate the total width needed to draw words
    for (char c : text) {
        CharMatrix &charMatrix = font.font_GetCharMatrix(c);
        totalWidth += charMatrix.width + 1;
    }
    // Accounting for the uncessary white space at the end
    totalWidth -= 1;

    layerGrid.resize(totalHeight);
    for (int i = 0; i < totalHeight; i++) {
        layerGrid[i].resize(totalWidth);
    }

    int currRowOffset = textRowOffset;
    int currColOffset = textColOffset;
    for (char c : text) {
        layer_placeChar(currRowOffset, currColOffset, c, color, backgroundColor, font);
        int charWidth = font.font_GetCharMatrix(c).width;
        if (charWidth > 0) {
            currColOffset += charWidth + 1;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < totalWidth; j++) {
            if (layerGrid[i][j] != color) {
                layerGrid[i][j] = backgroundColor;
            }
        }
    }
}

void Layer::placeTextInLayer(const std::string &text,
        DisplayFont &font,
        std::vector<int> &colorSequence,
        const int backgroundColor,
        int textRowOffset, int textColOffset) {
    scrollOffset = 0;
    layer_clearLayer();
    int totalWidth = 0;
    int totalHeight = height;
    // Calculate the total width needed to draw words
    for (char c : text) {
        CharMatrix &charMatrix = font.font_GetCharMatrix(c);
        totalWidth += charMatrix.width + 1;
    }
    // Account for space at end
    totalWidth -= 1;

    layerGrid.resize(totalHeight);
    for (int i = 0; i < totalHeight; i++) {
        layerGrid[i].resize(totalWidth);
    }
    
    size_t currColor = 0;
    int currRowOffset = textRowOffset;
    int currColOffset = textColOffset;
    for (char c : text) {
        if (c == ' ') {
            currColor += (currColor + 1) % colorSequence.size();
        }
        layer_placeChar(currRowOffset, currColOffset, c, colorSequence[currColor], backgroundColor, font);
        int charWidth = font.font_GetCharMatrix(c).width;
        if (charWidth > 0) {
            currColOffset += charWidth + 1;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < totalWidth; j++) {
            if (layerGrid[i][j] == 0) {
                layerGrid[i][j] = backgroundColor;
            }
        }
    }
}
void Layer::updateLayer() {
    if (layerGrid[0].size() <= 32) {
        return;
    }

    scrollOffset += 1;
    scrollOffset = scrollOffset % (layerGrid[0].size());
}

void Layer::fillWithColor(int color, bool animate) {
    std::vector<int> colors { color };
    fillWithColor(colors, animate);
}

void Layer::fillWithColor(std::vector<int> &colorSequence, bool animate) {
    size_t currColor = 0;
    if (animate) {}
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            layerGrid[i][j] = colorSequence[currColor];
            currColor = (currColor + 1) % colorSequence.size(); 
        }
    }
}

const std::vector<std::vector<int>>& Layer::getLayerGrid() {
    return layerGrid;
}
