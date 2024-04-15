#include "displayFont.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

DisplayFont::DisplayFont() {

}

DisplayFont::~DisplayFont() {

}

void DisplayFont::font_loadPPMFile(const std::filesystem::path &p) {
    std::string filename = p.filename().string();
    std::size_t pos = filename.find(".pbm");
    if (std::string::npos == pos) {
        return;
    }
    unsigned long num = std::stoul(filename.substr(0,pos));
    if (num > UINT8_MAX) {
        return;
    }
    char c = static_cast<char>(num);

    std::ifstream ppmFile(p);
    std::string line;

    // Getting rid of the header files
    std::getline(ppmFile, line);
    std::getline(ppmFile, line);

    // Code below based on stack overflow link
    // https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    int width, height;
    std::getline(ppmFile, line);
    std::istringstream iss(line);
    iss >> width >> height;
    std::getline(ppmFile, line);

    fontMatrices[c].width = width;
    fontMatrices[c].height = height;

    // Constructing the dot matrix corresponding to character
    // We can optimize this later by turning this into a a flat vector instead
    // of a square matrix
    fontMatrices[c].matrix.resize(height);
    for (int row = 0; row < height; row++) {
        fontMatrices[c].matrix[row].resize(width);
        if (line.size() == 0) {
            continue;
        }
        for (int col = 0; col < width; col++) {
            int lineIndex = row * width + col;
            char value = line[lineIndex];
            fontMatrices[c].matrix[row][col] = value == '1';
        }
    }
}

void DisplayFont::font_LoadFont(std::filesystem::path fontDirPath) {
    if (!std::filesystem::is_directory(fontDirPath)) {
        return;
    }
    // Reset font so we don't use previously loaded fonts
    for (auto &cm : fontMatrices) {
        cm.height = 0;
        cm.width = 0;
    }
    for (auto const& dirEntry : std::filesystem::directory_iterator{fontDirPath}) {
        font_loadPPMFile(dirEntry.path());
    }
}

CharMatrix &DisplayFont::font_GetCharMatrix(char c) {
    static CharMatrix emptyCharMatrix;
    if (c < ASCII_START || c > ASCII_END) {
        return emptyCharMatrix;
    }
    return fontMatrices[c];
}
