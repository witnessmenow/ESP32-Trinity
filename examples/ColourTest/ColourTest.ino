#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>

RGB64x32MatrixPanel_I2S_DMA dma_display;

uint16_t myBLACK = dma_display.color565(0, 0, 0);
uint16_t myWHITE = dma_display.color565(255, 255, 255);
uint16_t myRED = dma_display.color565(255, 0, 0);
uint16_t myGREEN = dma_display.color565(0, 255, 0);
uint16_t myBLUE = dma_display.color565(0, 0, 255);

void setup() {
  // put your setup code here, to run once:
  dma_display.begin();
  dma_display.setPanelBrightness(MATRIX_WIDTH);
  dma_display.clearScreen();
  dma_display.fillScreen(myWHITE);
  
}

void loop() {
  delay(5000);
  dma_display.fillScreen(myBLACK);
  delay(5000);
  dma_display.fillScreen(myBLUE);
  delay(5000);
  dma_display.fillScreen(myRED);
  delay(5000);
  dma_display.fillScreen(myGREEN);
  delay(5000);
  dma_display.fillScreen(myWHITE);

}
