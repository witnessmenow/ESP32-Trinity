#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>

RGB64x32MatrixPanel_I2S_DMA dma_display;
uint16_t myWHITE = dma_display.color565(255, 255, 255);

void setup() {
  // put your setup code here, to run once:
  dma_display.begin();
  dma_display.setPanelBrightness(MATRIX_WIDTH);
  dma_display.clearScreen();
  dma_display.fillScreen(myWHITE);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
