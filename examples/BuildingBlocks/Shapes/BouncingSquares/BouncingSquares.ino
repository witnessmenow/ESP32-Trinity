/*******************************************************************
    Bouncing squares of different colours and sizes

    Based on mrfaptastic's example in ESP32-HUB75-MatrixPanel-I2S-DMA
    https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/blob/master/examples/BouncingSquares/BouncingSquares.ino

    Parts Used:
      ESP32 Trinity - https://github.com/witnessmenow/ESP32-Trinity
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

// -------------------------------
// -------   Other Config   ------
// -------------------------------

const int numSquares = 20;
const int minSquareSize = 2;
const int maxSquareSize = 10;

const int refreshDelay = 20;

//------------------------------------------------------------------------------------------------------------------

MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myDARK = dma_display->color565(64, 64, 64);
uint16_t myWHITE = dma_display->color565(192, 192, 192);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);
uint16_t myCYAN = dma_display->color565(0, 255, 255);
uint16_t myMAGENTA = dma_display->color565(255, 0, 255);
uint16_t myYELLOW = dma_display->color565(255, 255, 0);

uint16_t colours[8] = { myDARK, myWHITE, myRED, myGREEN, myBLUE, myCYAN, myMAGENTA, myYELLOW };

struct Square
{
  float xpos, ypos;
  float velocityx;
  float velocityy;
  uint16_t square_size;
  uint16_t colour;
};

Square Squares[numSquares];

void displaySetup() {
  HUB75_I2S_CFG mxconfig(
    panelResX,   // Module width
    panelResY,   // Module height
    panel_chain  // Chain length
  );

  // This is how you enable the double buffer.
  // Double buffer can help with animation heavy projects
  mxconfig.double_buff = true;

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

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
}

void setup()
{
  Serial.begin(115200);
  displaySetup();

  // Create some Squares
  for (int i = 0; i < numSquares; i++)
  {
    Squares[i].square_size = random(minSquareSize, maxSquareSize);
    Squares[i].xpos = random(0, dma_display->width() - Squares[i].square_size);
    Squares[i].ypos = random(0, dma_display->height() - Squares[i].square_size);
    Squares[i].velocityx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    Squares[i].velocityy = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    Squares[i].colour = colours[i % 8];
  }
}

void loop()
{
  dma_display->clearScreen();

  for (int i = 0; i < numSquares; i++)
  {
    // Draw rect and then calculate
    dma_display->fillRect(Squares[i].xpos, Squares[i].ypos, Squares[i].square_size, Squares[i].square_size, Squares[i].colour);

    if (Squares[i].square_size + Squares[i].xpos >= dma_display->width()) {
      Squares[i].velocityx *= -1;
    }
    else if (Squares[i].xpos <= 0) {
      Squares[i].velocityx = abs (Squares[i].velocityx);
    }

    if (Squares[i].square_size + Squares[i].ypos >= dma_display->height()) {
      Squares[i].velocityy *= -1;
    }
    else if (Squares[i].ypos <= 0) {
      Squares[i].velocityy = abs (Squares[i].velocityy);
    }

    Squares[i].xpos += Squares[i].velocityx;
    Squares[i].ypos += Squares[i].velocityy;
  }

  dma_display->flipDMABuffer();
  delay(refreshDelay);
}
