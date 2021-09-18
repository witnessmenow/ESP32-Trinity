/*******************************************************************
    An Example showing scolling text

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

#define ENABLE_DOUBLE_BUFFER 1 // This is a good example to show the difference the 
// double buffer makes, it doesn't flash as much
// comment this out to test without it

// See the "displaySetup" method for more display config options

//------------------------------------------------------------------------------------------------------------------

MatrixPanel_I2S_DMA *dma_display = nullptr;

// For scrolling Text
unsigned long isAnimationDue;
int delayBetweeenAnimations = 25; // Smaller == faster

int textXPosition = panelResX * panel_chain; // Will start off screen
int textYPosition = panelResY / 2 - 8; // center of screen - 8 (half of the text height)

String text = "ESP32 Trinity, by Brian Lough";

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

  // If you are using a 64x64 matrix you need to pass a value for the E pin
  // The trinity connects GPIO 18 to E.
  // This can be commented out for any smaller displays (but should work fine with it)
  mxconfig.gpio.e = 18;

#ifdef ENABLE_DOUBLE_BUFFER
  // This is how you enable the double buffer.
  // Double buffer can help with animation heavy projects
  mxconfig.double_buff = true;
#endif


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

  dma_display->fillScreen(myBLACK);

  dma_display->setTextSize(2);     // size 2 == 16 pixels high
  dma_display->setTextWrap(false); // N.B!! Don't wrap at end of line
  dma_display->setTextColor(myRED);
#ifdef ENABLE_DOUBLE_BUFFER
  dma_display->flipDMABuffer();
#endif

}

// Will be used in getTextBounds.
int16_t xOne, yOne;
uint16_t w, h;

void loop() {
  unsigned long now = millis();
  if (now > isAnimationDue)
  {
    isAnimationDue = now + delayBetweeenAnimations;

    textXPosition -= 1;

    // Checking is the very right of the text off screen to the left
    dma_display->getTextBounds(text, textXPosition, textYPosition, &xOne, &yOne, &w, &h);
    if (textXPosition + w <= 0)
    {
      textXPosition = 64;
    }

    dma_display->setCursor(textXPosition, textYPosition);

    // The display has to do less updating if you only black out the area
    // the text is
    //dma_display->fillScreen(myBLACK);
    dma_display->fillRect(0, textYPosition, dma_display->width(), 16, myBLACK);

    dma_display->print(text);
#ifdef ENABLE_DOUBLE_BUFFER
    dma_display->flipDMABuffer();
#endif

  }

}
