/*******************************************************************
    An example showing different size fonts in different colours

    For use with my ESP32 Trinity or I2S Matrix Shield.

    ESP32Trinity.com

    Parts Used:
    ESP32 D1 Mini * - https://s.click.aliexpress.com/e/_dSi824B
    ESP32 I2S Matrix Shield (From my Tindie) = https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/

      = Affilate

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

// Adafruit GFX library is a dependancy for the matrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

// -------------------------------------
// -------   Matrix Config   ------
// -------------------------------------

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 64     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another

// -----------------------------

MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

void displayReconfig() {
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
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


void setup() {

  Serial.begin(115200);

  displayReconfig();

}

void loop() {
}
