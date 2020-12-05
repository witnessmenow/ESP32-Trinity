// And example showing some tricks with placing text

// - Scrolling Text
// - Centered Text

#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>

RGB64x32MatrixPanel_I2S_DMA dma_display;

// For scrolling Text
unsigned long isAnimationDue;
int delayBetweeenAnimations = 25; // Smaller == faster
int scrollXMove = -1; //If positive it would scroll right

int textXPosition = 64; // Will start off screen
int textYPosition = 24; // center of screen - 8 (half of the text height)

String text = "How is this Nigel and Noycebru?";

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

  dma_display.setTextSize(2);     // size 2 == 16 pixels high
  dma_display.setTextWrap(false); // N.B!! Don't wrap at end of line
  dma_display.setTextColor(myRED);

  
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
    dma_display.getTextBounds(text, textXPosition, textYPosition, &xOne, &yOne, &w, &h);
    if(textXPosition + w <= 0)
    {
      textXPosition = 64;
    }
    
    dma_display.setCursor(textXPosition, textYPosition);

    // The display has to do less updating if you only black out the area
    // the text is
    //dma_display.fillScreen(myBLACK);
    dma_display.fillRect(0, textYPosition, dma_display.width(), 16, myBLACK);

    dma_display.print(text); 
    
  }

}
