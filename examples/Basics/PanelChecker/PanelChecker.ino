/*******************************************************************
    A modified version of the hello world code that is quick way to try
    out some of the library configurations to test what works best on your
    matrix panel.

    Usage:
    On startup touch none/either/both of the built-in touch pads of the Trinity
    to enable different modes
    
    T8:
    Not Touched -> clkphase defaults to true
    Touched -> mxconfig.clkphase = false;

    T9:
    Not Touched -> Does not set a special driver
    Touched -> mxconfig.driver = HUB75_I2S_CFG::FM6126A;

    If you want to try a different combination, touch the buttons and press
    reset.

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

// -------------------------------------
// -------   Matrix Config   ------
// -------------------------------------

const int panelResX = 64;      // Number of pixels wide of each INDIVIDUAL panel module.
const int panelResY = 64;     // Number of pixels tall of each INDIVIDUAL panel module.
const int panel_chain = 1;      // Total number of panels chained one to another

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
    panelResX,   // module width
    panelResY,   // module height
    panel_chain    // Chain length
  );


  int threshold = 40; //This is a decent value from testing

  touchRead(T8);
  touchRead(T9); //dumping the first reading

  delay(100);
  int value = touchRead(T8);
  Serial.print("T8 Value: ");
  Serial.println(value);
  bool touch8detected = value < threshold;

  value = touchRead(T9);
  Serial.print("T9 Value: ");
  Serial.println(value);
  bool touch9detected = value < threshold;

  mxconfig.gpio.e = 18;

  

  if (touch8detected) {
    // May or may not be needed depending on your matrix
    // Example of what needing it looks like:
    // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/134#issuecomment-866367216
    Serial.println("clkPhase set false");
    mxconfig.clkphase = false;
  }

  if (touch9detected) {
    Serial.println("Driver: HUB75_I2S_CFG::FM6126A");
    mxconfig.driver = HUB75_I2S_CFG::FM6126A;
  }

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
}


void setup() {

  Serial.begin(115200);

  displaySetup();

    dma_display->clearScreen();
  dma_display->fillScreen(myBLACK);
  dma_display->setTextWrap(false);

  dma_display->setTextSize(1);     // size 1 == 8 pixels high
  dma_display->setTextColor(myBLUE);
  dma_display->setCursor(0, 0);
  dma_display->print("Hello");

  dma_display->setTextSize(2);     // size 2 == 16 pixels high
  dma_display->setTextColor(myGREEN);
  dma_display->setCursor(0, 8);
  dma_display->print("Hello");

  dma_display->setTextSize(3);     // size 3 == 24 pixels high
  dma_display->setTextColor(myRED);
  dma_display->setCursor(0, 24);
  dma_display->print("Hello");

}

void loop() {
}
