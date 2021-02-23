// An example showing different size fonts in different colours


#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
MatrixPanel_I2S_DMA dma_display;

uint16_t myBLACK = dma_display.color565(0, 0, 0);
uint16_t myWHITE = dma_display.color565(255, 255, 255);
uint16_t myRED = dma_display.color565(255, 0, 0);
uint16_t myGREEN = dma_display.color565(0, 255, 0);
uint16_t myBLUE = dma_display.color565(0, 0, 255);

void setup() {

  Serial.begin(115200);

  dma_display.begin();

  dma_display.clearScreen();
  dma_display.fillScreen(myBLACK);
  dma_display.setTextWrap(false);
  
  dma_display.setTextSize(1);     // size 1 == 8 pixels high
  dma_display.setTextColor(myBLUE);
  dma_display.setCursor(0, 0);
  dma_display.print("Hello");

  dma_display.setTextSize(2);     // size 2 == 16 pixels high
  dma_display.setTextColor(myGREEN);
  dma_display.setCursor(0, 8);
  dma_display.print("Hello");

  dma_display.setTextSize(3);     // size 3 == 24 pixels high
  dma_display.setTextColor(myRED);
  dma_display.setCursor(0, 24);
  dma_display.print("Hello");

}

void loop() {

}
