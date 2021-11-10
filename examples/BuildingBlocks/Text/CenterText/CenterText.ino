/*******************************************************************
    An Example to show how to center text.

    Parts Used:
      ESP32 Trinity - https://github.com/witnessmenow/ESP32-Trinity

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/

    Written by Brian Lough
      YouTube: https://www.youtube.com/brianlough
      Tindie: https://www.tindie.com/stores/brianlough/
      Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA


// ----------------------------
// Dependency Libraries - each one of these will need to be installed.
// ----------------------------

// Adafruit GFX library is a dependency for the matrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

// --------------------------------
// -------   Matrix Config   ------
// --------------------------------

const int panelResX = 64;   // Number of pixels wide of each INDIVIDUAL panel module.
const int panelResY = 64;   // Number of pixels tall of each INDIVIDUAL panel module.
const int panel_chain = 1;  // Total number of panels chained one to another.

// See the "displaySetup" method for more display config options

//------------------------------------------------------------------------------------------------------------------

MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

void displaySetup() {
  HUB75_I2S_CFG mxconfig(
    panelResX,   // Module width
    panelResY,   // Module height
    panel_chain  // Chain length
  );

  // If you are using a 64x64 matrix you need to pass a value for the E pin
  // The trinity connects GPIO 18 to E.
  // This can be commented out for any smaller displays (but should work fine with it)
  mxconfig.gpio.e = 18;


  // May or may not be needed depending on your matrix
  // Example of what needing it looks like:
  // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/134#issuecomment-866367216
  mxconfig.clkphase = false;

  // Some matrix panels use different ICs for driving them and some of them have strange quirks.
  // If the display is not working right, try this.
  //mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
}

void setup() {

  Serial.begin(115200);

  displaySetup();

  dma_display->clearScreen();
  dma_display->fillScreen(myBLACK);

  dma_display->setTextSize(2);     // size 2 == 16 pixels high
  dma_display->setTextWrap(false); // N.B!! Don't wrap at end of line
  dma_display->setTextColor(myBLUE);

  // Pointers to this variable will be passed into getTextBounds,
  // they will be updated from inside the method
  int16_t xOne, yOne;
  uint16_t w, h;

  // This method updates the variables with what width (w) and height (h)
  // the give text will have.

  dma_display->getTextBounds("HI", 0, 0, &xOne, &yOne, &w, &h);

  int xPosition = dma_display->width() / 2 - w / 2 + 1;
  // The 1 at the end is to combat what seems like a bug somewhere:
  // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/43

  dma_display->setCursor(xPosition, 24);
  dma_display->print("HI");

  delay(5000);
  dma_display->fillScreen(myBLACK);
  dma_display->getTextBounds("BYE", 0, 0, &xOne, &yOne, &w, &h);
  xPosition = dma_display->width() / 2 - w / 2 + 1;

  dma_display->setCursor(xPosition, 24);
  dma_display->print("BYE");

}

void loop() {

}
