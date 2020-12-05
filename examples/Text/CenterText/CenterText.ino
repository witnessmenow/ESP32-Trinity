// And example showing how to center text


#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
MatrixPanel_I2S_DMA dma_display;

uint16_t myBLACK = dma_display.color565(0, 0, 0);
uint16_t myWHITE = dma_display.color565(255, 255, 255);
uint16_t myRED = dma_display.color565(255, 0, 0);
uint16_t myGREEN = dma_display.color565(0, 255, 0);
uint16_t myBLUE = dma_display.color565(0, 0, 255);

int16_t xOne, yOne;
uint16_t w, h;

void setup() {

  Serial.begin(115200);

  dma_display.begin();

  dma_display.clearScreen();
  dma_display.fillScreen(myBLACK);

  dma_display.setTextSize(2);     // size 2 == 16 pixels high
  dma_display.setTextWrap(false); // N.B!! Don't wrap at end of line
  dma_display.setTextColor(myBLUE);


  dma_display.getTextBounds("HI", 0, 0, &xOne, &yOne, &w, &h);
  int xPosition = dma_display.width()/2 - w/2 + 1;
  // The 1 at the end is to combat what seems like a bug somewhere: 
  // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/43

  dma_display.setCursor(xPosition, 24);
  dma_display.print("HI");

  delay(5000);
  dma_display.fillScreen(myBLACK);
  dma_display.getTextBounds("BYE", 0, 0, &xOne, &yOne, &w, &h);
  xPosition = dma_display.width()/2 - w/2 + 1; 

  dma_display.setCursor(xPosition, 24);
  dma_display.print("BYE");

}

void loop() {

}
