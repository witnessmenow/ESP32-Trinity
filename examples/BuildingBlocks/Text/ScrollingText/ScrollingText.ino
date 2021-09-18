/*******************************************************************
    An Example showing scolling text

    For use with the ESP32 Trinity or my I2S Matrix Shield.

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
// Standard Libraries - Already Installed if you have ESP32 set up
// ----------------------------


// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA


// -------------------------------------
// -------   Matrix Config   ------
// -------------------------------------

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another

// -----------------------------

MatrixPanel_I2S_DMA *dma_display = nullptr;

// For scrolling Text
unsigned long isAnimationDue;
int delayBetweeenAnimations = 25; // Smaller == faster
int scrollXMove = -1; //If positive it would scroll right

int textXPosition = PANEL_RES_X * PANEL_CHAIN; // Will start off screen
int textYPosition = PANEL_RES_Y - 8; // center of screen - 8 (half of the text height)

String text = "How is this Nigel and Noycebru?";

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

void setup() {

  Serial.begin(115200);
  
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );

  mxconfig.gpio.e = 18;

  // May or may not be needed depending on your matrix
  // Example of what needing it looks like:
  // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/134#issuecomment-866367216
  mxconfig.clkphase = false;

  //mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  
  dma_display->fillScreen(myBLACK);

  dma_display->setTextSize(2);     // size 2 == 16 pixels high
  dma_display->setTextWrap(false); // N.B!! Don't wrap at end of line
  dma_display->setTextColor(myRED);

  
}

// Will be used in getTextBounds.
int16_t xOne, yOne;
uint16_t w, h;

void loop() {
  unsigned long now = millis();
  if(now > isAnimationDue)
  {
    isAnimationDue = now + delayBetweeenAnimations;

    textXPosition += scrollXMove;
    
    // Checking is the very right of the text off screen to the left
    dma_display->getTextBounds(text, textXPosition, textYPosition, &xOne, &yOne, &w, &h);
    if(textXPosition + w <= 0)
    {
      textXPosition = 64;
    }
    
    dma_display->setCursor(textXPosition, textYPosition);

    // The display has to do less updating if you only black out the area
    // the text is
    //dma_display->fillScreen(myBLACK);
    dma_display->fillRect(0, textYPosition, dma_display->width(), 16, myBLACK);

    dma_display->print(text); 
    
  }

}
