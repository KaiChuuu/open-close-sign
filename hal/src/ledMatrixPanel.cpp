/****************************************************
  CITED WORK USED MAKE PANEL DISPLAY
  - https://github.com/MontreaI/BeagleBone-Green-Adafruit-16x32-LED-Matrix-Sample-Code/tree/master 
  - https://cdn-learn.adafruit.com/downloads/pdf/32x16-32x32-rgb-led-matrix.pdf
  - https://opencoursehub.cs.sfu.ca/bfraser/grav-cms/cmpt433/links/files/2023-student-howtos/UsbControllerAnd16x32LEDPanel.pdf
****************************************************/

#include "hal/ledMatrixPanel.h"
#include <cstdio>

#define GPIO_PATH "/sys/class/gpio/"
#define S_IWRITE "S_IWUSR"

#define DELAY_IN_SEC 0
#define DELAY_IN_US 5

// GPIO PIN DEFINITION
#define RED_PIN_1 "78"
#define GREEN_PIN_1 "70"
#define BLUE_PIN_1 "8"

#define RED_PIN_2 "71"
#define GREEN_PIN_2 "72"
#define BLUE_PIN_2 "73"

#define A_PIN "75"
#define B_PIN "74"
#define C_PIN "77"

#define CLOCK_PIN "79" //Arrival of each bit of data
#define LATCH_PIN "76" //Marks the end of a row of data 
#define OE_PIN "80"    //Switches LED off when transitioning from one row to the next

static int fileRed1, fileBlue1, fileGrn1;
static int fileRed2, fileBlue2, fileGrn2;
static int fileClk, fileLatch, fileOE;
static int fileA, fileB, fileC;

// LED Panel Values in Matrix
static int panel[32][16];

//Depending on beaglebone, might need to be configured manually and ignored.
void LedMatrixPanel::panel_exportPins(){
    board.beagle_updateFile(GPIO_PATH "export", RED_PIN_1);
    board.beagle_updateFile(GPIO_PATH "export", GREEN_PIN_1);
    board.beagle_updateFile(GPIO_PATH "export", BLUE_PIN_1);
    board.beagle_updateFile(GPIO_PATH "export", RED_PIN_2);
    board.beagle_updateFile(GPIO_PATH "export", GREEN_PIN_2);
    board.beagle_updateFile(GPIO_PATH "export", BLUE_PIN_2);
    board.beagle_updateFile(GPIO_PATH "export", A_PIN);
    board.beagle_updateFile(GPIO_PATH "export", B_PIN);
    board.beagle_updateFile(GPIO_PATH "export", C_PIN);
    board.beagle_updateFile(GPIO_PATH "export", CLOCK_PIN);
    board.beagle_updateFile(GPIO_PATH "export", LATCH_PIN);
    board.beagle_updateFile(GPIO_PATH "export", OE_PIN);
}

void LedMatrixPanel::panel_setPinDirection(){
    board.beagle_updateFile(GPIO_PATH "gpio" RED_PIN_1 "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" GREEN_PIN_1 "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" BLUE_PIN_1 "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" RED_PIN_2 "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" GREEN_PIN_2 "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" BLUE_PIN_2 "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" A_PIN "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" B_PIN "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" C_PIN "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" CLOCK_PIN "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" LATCH_PIN "/direction", "out");
    board.beagle_updateFile(GPIO_PATH "gpio" OE_PIN "/direction", "out");
}

void LedMatrixPanel::panel_openFiles(){
    fileRed1 = open(GPIO_PATH "gpio" RED_PIN_1 "/value", O_WRONLY, S_IWRITE);
    fileGrn1 = open(GPIO_PATH "gpio" GREEN_PIN_1 "/value", O_WRONLY, S_IWRITE);
    fileBlue1 = open(GPIO_PATH "gpio" BLUE_PIN_1 "/value", O_WRONLY, S_IWRITE);
    
    fileRed2 = open(GPIO_PATH "gpio" RED_PIN_2 "/value", O_WRONLY, S_IWRITE);
    fileGrn2 = open(GPIO_PATH "gpio" GREEN_PIN_2 "/value", O_WRONLY, S_IWRITE);
    fileBlue2 = open(GPIO_PATH "gpio" BLUE_PIN_2 "/value", O_WRONLY, S_IWRITE);
    
    fileClk = open(GPIO_PATH "gpio" CLOCK_PIN "/value", O_WRONLY, S_IWRITE);
    fileLatch = open(GPIO_PATH "gpio" LATCH_PIN "/value", O_WRONLY, S_IWRITE);
    fileOE = open(GPIO_PATH "gpio" OE_PIN "/value", O_WRONLY, S_IWRITE);

    fileA = open(GPIO_PATH "gpio" A_PIN "/value", O_WRONLY, S_IWRITE);
    fileB = open(GPIO_PATH "gpio" B_PIN "/value", O_WRONLY, S_IWRITE);
    fileC = open(GPIO_PATH "gpio" C_PIN "/value", O_WRONLY, S_IWRITE);
}

void LedMatrixPanel::panel_configurePins(){
    //panel_exportPins();    
    panel_setPinDirection();
    panel_openFiles();
}

//Update Panel
void LedMatrixPanel::panel_setPixel(int row, int column, int colour){
    if ( row < 0 || row >= 16 || column < 0 || column >= 32) return;

    panel[column][row] = colour;
}

void LedMatrixPanel::panel_initializeMatrix(){
    memset(panel, 0, sizeof(panel));
    
    /*
    //Temporarily to see color
    for ( int i = 0; i < 16; i++ ) {
        panel_setPixel(i, i, 1);
        panel_setPixel(i, 32-1-i , 2);
    }
    */
}

void panel_bitBangClock(){
    lseek(fileClk, 0, SEEK_SET);
    write(fileClk, "1", 1);
    lseek(fileClk, 0, SEEK_SET);
    write(fileClk, "0", 1);
}

void panel_generateLatchPins(){
    lseek(fileLatch, 0, SEEK_SET);
    write(fileLatch, "1", 1);
    lseek(fileLatch, 0, SEEK_SET);
    write(fileLatch, "0", 1);
}

void panel_bitsFromInt(int* arr, int input){
    arr[0] = input & 1;
    arr[1] = input & 2;
    arr[1] = arr[1] >> 1;
    arr[2] = input & 4;
    arr[2] = arr[2] >> 2;
}

void panel_setRow(int row){
    // Convert row single bits from int
    int arr[3] = {0, 0, 0};
    panel_bitsFromInt(arr, row);

    // Write on the row pins
    char aVal[2];
    sprintf(aVal, "%d", arr[0]);
    lseek(fileA, 0, SEEK_SET);
    write(fileA, aVal, 1);

    char bVal[2];
    sprintf(bVal, "%d", arr[1]);
    lseek(fileB, 0, SEEK_SET);
    write(fileB, bVal, 1);

    char cVal[2];
    sprintf(cVal, "%d", arr[2]);
    lseek(fileC, 0, SEEK_SET);
    write(fileC, cVal, 1);
}

void panel_setTopColor(int color){
    int arr[3] = {0, 0, 0};
    panel_bitsFromInt(arr, color);

    // Write on the colour pins
    char redVal[2];
    sprintf(redVal, "%d", arr[0]);
    lseek(fileRed1, 0, SEEK_SET);
    write(fileRed1, redVal, 1);

    char greenVal[2];
    sprintf(greenVal, "%d", arr[1]);
    lseek(fileGrn1, 0, SEEK_SET);
    write(fileGrn1, greenVal, 1);

    char blueVal[2];
    sprintf(blueVal, "%d", arr[2]);
    lseek(fileBlue1, 0, SEEK_SET);
    write(fileBlue1, blueVal, 1);  
}

void panel_setBottomColor(int color){
    int arr[3] = {0, 0, 0};
    panel_bitsFromInt(arr, color);

    // Write on the colour pins
    char redVal2[2];
    sprintf(redVal2, "%d", arr[0]);
    lseek(fileRed2, 0, SEEK_SET);
    write(fileRed2, redVal2, 1);

    char greenVal2[2];
    sprintf(greenVal2, "%d", arr[1]);
    lseek(fileGrn2, 0, SEEK_SET);
    write(fileGrn2, greenVal2, 1);

    char blueVal2[2];
    sprintf(blueVal2, "%d", arr[2]);
    lseek(fileBlue2, 0, SEEK_SET);
    write(fileBlue2, blueVal2, 1);  
}

void LedMatrixPanel::panel_refreshPanel(){
    for(int row = 0; row < 8; row++){
        lseek(fileOE, 0, SEEK_SET);
        write(fileOE, "1", 1);
        panel_setRow(row);
        for(int col = 0; col < 32; col++){
            panel_setTopColor(panel[col][row]);
            panel_setBottomColor(panel[col][row+8]);
            panel_bitBangClock();
        }
        panel_generateLatchPins();
        lseek(fileOE, 0, SEEK_SET);
        write(fileOE, "0", 1);
        struct timespec reqDelay = {DELAY_IN_SEC, DELAY_IN_US}; // sleep for delay
    	nanosleep(&reqDelay, (struct timespec *) NULL);
    }
}

void LedMatrixPanel::panel_init(){
    panel_configurePins();

    panel_initializeMatrix();
}

void LedMatrixPanel::panel_cleanup(){

}

void LedMatrixPanel::panel_printPanel() {
    printf("Panel:\n");
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 32; j++) {
            printf("%d", panel[j][i]);
        }
        printf("\n");
    }
}
