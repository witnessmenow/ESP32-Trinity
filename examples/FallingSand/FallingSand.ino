/*******************************************************************
    Falling Sand Project for i2s matrix library

    Original Written by @tetra3dprint
    Twiter: https://twitter.com/tetra3dprint
    Github: https://github.com/tetra3dprint

    Adapted to i2s Matrix library by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow

    Parts:
    ESP32 D1 Mini * - https://s.click.aliexpress.com/e/_dSi824B
    ESP32 I2S Matrix Shield (From Brian's Tindie) = https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/

 *  * = Affilate
    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/

    I (Tetra3dPrint) used this awesome tool to convert images into arrays for the pallet
    https://littlevgl.com/image-to-c-array

 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#include <Wire.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <MPU6050_tockn.h>
// This library is for interfacing with the MPU6050 Accelerometer

// Can be installed from the library manager
// https://github.com/tockn/MPU6050_tockn

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

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 64     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another

// -------------------------------------
// -------   Other Config Config   ------
// -------------------------------------

#define SHOW_TIME 10

#define MAX_FPS      100 // Maximum redraw rate, frames/second
#define DISPLAY_FPS 50

#define SHOW_FPS

#define PIXEL_COUNT 1500

//------------------------------------------------------------------------------------------------------------------


// OTT FPS counter
#ifdef SHOW_FPS
#include "FPS.h"
FPS fps;
#endif
//------------------------------------------------------------------------------------------------------------------

MPU6050 mpu6050(Wire);
int t = 0;

MatrixPanel_I2S_DMA *dma_display = nullptr;

struct pixel {
  float x;
  float y;
  float dx;
  float dy;
  float deltaScale;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t ix;
  uint8_t iy;
};


pixel pixels[PIXEL_COUNT];
uint16_t screenPixels[PANEL_RES_X][PANEL_RES_Y];


// Second Core Stuff

float x, y, ax, ay, newX, newY, newDx, newDy, tDx, tDy;
uint8_t tryAgain;
#define ACCELERARION (float)0.1
#define THRESHOLD (float)0.02
uint32_t        prevTime   = 0;

void processSand(void *param) {

  //Default ESP32 i2c pins (21,22)
  Wire.begin(SDA, SCL, 400000);
  mpu6050.begin();


  while (true) {
    // Limit the animation frame rate to MAX_FPS.  Because the subsequent sand
    // calculations are non-deterministic (don't always take the same amount
    // of time, depending on their current states), this helps ensure that
    // things like gravity appear constant in the simulation.
    uint32_t t;
    while (((t = micros()) - prevTime) < (1000000L / MAX_FPS));
    prevTime = t;

    mpu6050.update();

    x = mpu6050.getAccX();
    y = mpu6050.getAccY();

    ax = y;
    ay = -x;

    for (uint16_t p = 0; p < PIXEL_COUNT; p++) {
      tryAgain = 3;

      while (tryAgain > 0) {
        if (tryAgain == 3) {
          newDx = ((pixels[p].dx + (ax * ACCELERARION))) * pixels[p].deltaScale;
          newDy = ((pixels[p].dy + (ay * ACCELERARION))) * pixels[p].deltaScale;
        } else if (tryAgain == 2) {
          if (abs(pixels[p].dx) > abs(pixels[p].dx) ) {
            newDx = ((pixels[p].dx + ax) * .5) * pixels[p].deltaScale;
            newDy = 0;
          } else {
            newDx = 0;
            newDy = ((pixels[p].dy * ACCELERARION * .8 + ay)) * pixels[p].deltaScale;
          }
        } else if (tryAgain == 1) {
          if (abs(pixels[p].dx) > abs(pixels[p].dx) ) {
            newDx = 0;
            newDy = ((pixels[p].dy * ACCELERARION * .8 + ay)) * pixels[p].deltaScale;
          } else {
            newDx = ((pixels[p].dx + ax) * .5) * pixels[p].deltaScale;
            newDy = 0;
          }
        }
        newX = (float)pixels[p].x + newDx;
        newY = (float)pixels[p].y + newDy;
        if (newX > PANEL_RES_X - 1) newX = PANEL_RES_X - 1;
        if (newX < 0) newX = 0;
        if (newY > PANEL_RES_Y - 1) newY = PANEL_RES_Y - 1;
        if (newY < 0) newY = 0;
        uint8_t x = (uint8_t)newX;
        uint8_t y = (uint8_t)newY;
        uint16_t targetPixel = screenPixels[x][y];


        if (targetPixel == 65535) { // can move
          tryAgain = 0;
          screenPixels[pixels[p].ix][pixels[p].iy] = 65535;
          pixels[p].x = newX;
          pixels[p].y = newY;
          pixels[p].dx = newDx;
          pixels[p].dy = newDy;
          pixels[p].ix = (uint8_t)pixels[p].x;
          pixels[p].iy = (uint8_t)pixels[p].y;
          screenPixels[pixels[p].ix][pixels[p].iy] = p;
        } else if (targetPixel == p) { // same pixel
          tryAgain = 0;
          pixels[p].x = newX;
          pixels[p].y = newY;
          pixels[p].dx = newDx;
          pixels[p].dy = newDy;
        } else {
          // can't move
          if (tryAgain == 3 && targetPixel != 60000) {
            pixels[targetPixel].dx += newDx * 0.5;
            pixels[targetPixel].dy += newDy * 0.5;
            //pixels[p].dx = newDx * 0.5;
            //pixels[p].dy = newDy * 0.5;
          }
          tryAgain--;
          if (tryAgain == 1) {
            pixels[p].dx = 0;
            pixels[p].dy = 0;
          }
        }
      }
    }

  }
}

//------------------------------------------------------------------------------------------------------------------
void setup() {

  //Serial.begin(115200); // I only enable this for debug
  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );
  
  mxconfig.double_buff = true;
  mxconfig.gpio.e = 18;

  // May or may not be needed depending on your matrix
  // Example of what needing it looks like:
  // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/134#issuecomment-866367216
  mxconfig.clkphase = false;

  //mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();


  for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
      screenPixels[x][y] = 65535;
    }
  }

  for (uint8_t z = 6; z < PANEL_RES_X; z += 16) {
    for (uint8_t q = 6; q < PANEL_RES_Y; q += 16) {
      for (uint8_t y = 0; y < 6; y++) {
        for (uint8_t x = 0; x < 6; x++) {
          screenPixels[x + z][y + q] = 60000;
        }
      }
    }
  }



  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint16_t tri = PIXEL_COUNT / 3;
  for (uint16_t p = 0; p < PIXEL_COUNT; p++) {
    //if (random(0, 2) > 0 ) {
    if (p < tri) {
      r = 255;
      g = 0;
      b = 0;
    } else if (p > tri && p < (tri * 2)) {
      r = 0;
      g = 255;
      b = 0;
    } else {
      r = 0;
      g = 0;
      b = 255;
    }


    pixels[p] = pixel{
      random(0, PANEL_RES_X),
      random(0, PANEL_RES_Y),
      0,
      0,
      (float)random(90 , 100) / 100,
      r,
      g,
      b,
      0,
      0
    };

    pixels[p].ix = (uint8_t)pixels[p].x;
    pixels[p].iy = (uint8_t)pixels[p].y;
    if ((screenPixels[pixels[p].ix][pixels[p].iy] < PIXEL_COUNT || screenPixels[pixels[p].ix][pixels[p].iy] == 60000)) {
      p--;
      continue;
    }
    screenPixels[pixels[p].ix][pixels[p].iy] = p;
  }
  //mpu6050.calcGyroOffsets(true);


  dma_display->setPanelBrightness(32);
  dma_display->fillScreen(0x0);
  dma_display->setTextColor(0xFFFF);
  dma_display->setTextSize(1);
  dma_display->showDMABuffer();

  delay(1000);

  TaskHandle_t xHandle = NULL;
  xTaskCreatePinnedToCore(processSand, "ProcessSand1", 5000, 0, (2 | portPRIVILEGE_BIT), &xHandle, 0);
#ifdef SHOW_FPS
  fps.getReady();
#endif
}

uint32_t loopPreviousT;

void loop() {

  uint32_t t;
  while (((t = micros()) - loopPreviousT) < (1000000L / DISPLAY_FPS));
  loopPreviousT = t;
  dma_display->flipDMABuffer();
  dma_display->fillScreen(0x0);
  //  mpu6050.update();
  //
  //  x = mpu6050.getAccX();
  //  y = mpu6050.getAccY();
  //  z = mpu6050.getAccZ();
  //  //updatePixels(x, y);
  //  updatePixels(y, -x);
  for (int p = 0; p < PIXEL_COUNT; p++) {
    dma_display->drawPixelRGB888(pixels[p].ix, pixels[p].iy, pixels[p].r, pixels[p].g, pixels[p].b);
  }

  for (uint8_t z = 6; z < PANEL_RES_X; z += 16) {
    for (uint8_t q = 6; q < PANEL_RES_Y; q += 16) {
      for (uint8_t y = 0; y < 6; y++) {
        for (uint8_t x = 0; x < 6; x++) {
          dma_display->drawPixelRGB888(x + z, y + q, 0xFF, 0xFF, 0x00);
        }
      }
    }
  }


#ifdef SHOW_FPS
  fps.tick();
  dma_display->setCursor(1, 1);
  dma_display->print("FPS:");  dma_display->print(fps.getStringFPS());
#endif
  dma_display->showDMABuffer();
}
