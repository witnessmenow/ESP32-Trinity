/*******************************************************************
    Displays gifs on a matrix display

    This loads the gifs from memory, this is the fastest way
    
    But speeds probably isn't super important for this, there is an example with the I2S matrix
    library tha loads the gifs from SPIFFS, which is probably a better way of doing it:
    https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/tree/master/examples/AnimatedGIFPanel

    Gettting the Gifs:
      - This is a decent source of low res gifs: https://betterttv.com/emotes/top
      - I resized the gifs using EzGif: https://ezgif.com/resize
      - Then converted them using this tool: https://github.com/bitbank2/image_to_c

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
// Include Other Tabs
// ----------------------------

#include "homer_tiny.h"
#include "fire.h"
#include "ThisisFine.h"
#include "cat.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA

#include <AnimatedGIF.h>
// Library for decoding GIFs on Arduino

// Search for "AnimatedGIF" in the Arduino Library manager
// https://github.com/bitbank2/AnimatedGIF

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
AnimatedGIF gif;

// Draw a line of image directly on the LCD
void GIFDraw(GIFDRAW *pDraw)
{
  uint8_t *s;
  uint16_t *d, *usPalette, usTemp[320];
  int x, y, iWidth;

  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line

  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2) // restore to background color
  {
    for (x = 0; x < iWidth; x++)
    {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }
  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency) // if transparency used
  {
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    int x, iCount;
    pEnd = s + pDraw->iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while (x < pDraw->iWidth)
    {
      c = ucTransparent - 1;
      d = usTemp;
      while (c != ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent) // done, stop
        {
          s--; // back up to treat it like transparent
        }
        else // opaque
        {
          *d++ = usPalette[c];
          iCount++;
        }
      } // while looking for opaque pixels
      if (iCount) // any opaque pixels?
      {
        for (int xOffset = 0; xOffset < iCount; xOffset++ ) {
          dma_display->drawPixel(x + xOffset, y, usTemp[xOffset]);
        }
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent)
          iCount++;
        else
          s--;
      }
      if (iCount)
      {
        x += iCount; // skip these
        iCount = 0;
      }
    }
  }
  else
  {
    s = pDraw->pPixels;
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    for (x = 0; x < pDraw->iWidth; x++)
    {
      dma_display->drawPixel(x, y, usPalette[*s++]);
    }
  }
} /* GIFDraw() */

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

  mxconfig.gpio.b1 = 26;
  mxconfig.gpio.b2 = 12;

  mxconfig.gpio.g1 = 27;
  mxconfig.gpio.g2 = 13;

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
  dma_display->fillScreen(dma_display->color565(0, 0, 0));
  gif.begin(LITTLE_ENDIAN_PIXELS);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gif.open((uint8_t *)homer_tiny, sizeof(homer_tiny), GIFDraw))
  {
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    while (gif.playFrame(true, NULL))
    {
    }
    gif.close();
  }

  for (int i = 0; i < 1; i++)
  {
    if (gif.open((uint8_t *)cat64, sizeof(cat64), GIFDraw))
    {
      Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
      while (gif.playFrame(true, NULL))
      {
      }
      gif.close();
    }
  }

  for (int i = 0; i < 10; i++)
  {
    if (gif.open((uint8_t *)ThisIsFine64, sizeof(ThisIsFine64), GIFDraw))
    {
      Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
      while (gif.playFrame(true, NULL))
      {
      }
      gif.close();
    }
  }

  // This Gif only has 3 frames, so it ends very quickly, so looping it
  for (int i = 0; i < 10; i++)
  {
    if (gif.open((uint8_t *)fire, sizeof(fire), GIFDraw))
    {
      Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
      while (gif.playFrame(true, NULL))
      {
      }
      gif.close();
    }
  }

}
