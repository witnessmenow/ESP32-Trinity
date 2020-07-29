#ifndef VIRTUAL_GFX_H
#define VIRTUAL_GFX_H

#include <Adafruit_GFX.h>

struct VirtualCoords {
  int16_t x;
  int16_t y;
};

class VirtualGFX : public Adafruit_GFX
{

  public:
    int16_t virtualResX;
    int16_t virtualResY;

    int16_t rows;
    int16_t cols;

    int16_t screenResX;
    int16_t screenResY;

    RGB64x32MatrixPanel_I2S_DMA *display;

    VirtualGFX(RGB64x32MatrixPanel_I2S_DMA &disp, int vRows, int vCols, int screenX, int screenY, int vResX, int vResY)
      : Adafruit_GFX(vResX, vResY)
    {
      this->display = &disp;
      rows = vRows;
      cols = vCols;
      screenResX = screenX;
      screenResY = screenY;

      virtualResX = vResX;
      virtualResY = vResY;
    }

    VirtualCoords getCoords(int16_t x, int16_t y);

    // equivalent methods of the matrix library so it can be just swapped out.
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void fillScreen(uint16_t color);                        // overwrite adafruit implementation
    void clearScreen() {
      fillScreen(0);
    }
    void drawPixelRGB565(int16_t x, int16_t y, uint16_t color);
    void drawPixelRGB888(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b);
    void drawPixelRGB24(int16_t x, int16_t y, rgb_24 color);
    void drawIcon (int *ico, int16_t x, int16_t y, int16_t cols, int16_t rows);

    uint16_t color444(uint8_t r, uint8_t g, uint8_t b) {
      return display->color444(r, g, b);
    }
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
      return display->color565(r, g, b);
    }
    uint16_t Color333(uint8_t r, uint8_t g, uint8_t b) {
      return display->Color333(r, g, b);
    }

  private:
    VirtualCoords coords;

}; // end Class header

inline VirtualCoords VirtualGFX::getCoords(int16_t x, int16_t y) {
  int16_t xOffset = (y / screenResY) * (cols * screenResX);
  coords.x = x + xOffset;
  coords.y = y % screenResY;
  return coords;
}

inline void VirtualGFX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  VirtualCoords coords = getCoords(x, y);
  this->display->drawPixel(coords.x, coords.y, color);
}

inline void VirtualGFX::fillScreen(uint16_t color)  // adafruit virtual void override
{
  // No need to map this.
  this->display->fillScreen(color);
}

inline void VirtualGFX::drawPixelRGB565(int16_t x, int16_t y, uint16_t color)
{
  VirtualCoords coords = getCoords(x, y);
  this->display->drawPixelRGB565( coords.x, coords.y, color);
}


inline void VirtualGFX::drawPixelRGB888(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b)
{
  VirtualCoords coords = getCoords(x, y);
  this->display->drawPixelRGB888( coords.x, coords.y, r, g, b);
}

inline void VirtualGFX::drawPixelRGB24(int16_t x, int16_t y, rgb_24 color)
{
  VirtualCoords coords = getCoords(x, y);
  this->display->drawPixelRGB24(coords.x, coords.y, color);
}

// need to recreate this one, as it wouldnt work to just map where it starts.
inline void VirtualGFX::drawIcon (int *ico, int16_t x, int16_t y, int16_t cols, int16_t rows) {
  int i, j;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      // This is a call to this libraries version of drawPixel
      // which will map each pixel, which is what we want.
      drawPixelRGB565 (x + j, y + i, ico[i * cols + j]);
    }
  }
}

#endif
