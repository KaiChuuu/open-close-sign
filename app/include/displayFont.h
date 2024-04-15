
#ifndef _DISPLAY_FONT_H_
#define _DISPLAY_FONT_H_

#include <array>
#include <filesystem>
#include <vector>

// Defines the range of ascii values in range of decimal values
#define ASCII_START 32
#define ASCII_END 126
#define ASCII_COUNT 256

class CharMatrix {
public:
    int height;
    int width;
    std::vector<std::vector<bool>> matrix;
};

class DisplayFont {
    std::array<CharMatrix, ASCII_COUNT> fontMatrices;
    void font_loadPPMFile(const std::filesystem::path &p);
public:
    DisplayFont();
    ~DisplayFont();
    void font_LoadFont(std::filesystem::path fontDirPath);
    CharMatrix &font_GetCharMatrix(char c); 
};


#endif
