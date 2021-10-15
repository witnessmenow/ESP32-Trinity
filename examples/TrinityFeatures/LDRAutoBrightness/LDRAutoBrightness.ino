// An example of changing the brightness of the panel
// based on the analog reading

// The idea would be to connect it to an LDR
// https://create.arduino.cc/projecthub/SBR/working-with-light-dependent-resistor-ldr-1ded4f


#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// placeholder for the matrix object
MatrixPanel_I2S_DMA *dma_display = nullptr;

#define PANEL_WIDTH 64
#define PANEL_HEIGHT 64

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

#define LDR_PIN 34 // Pin 34 is input only, so its a good pin to use for this purpose
unsigned long ldrCheckDue = 0;
int ldrDelay = 100;

#define MIN_INPUT 0
#define MAX_INPUT 4095

#define MIN_DISPLAY_BRIGHTNESS 10
#define MAX_DISPLAY_BRIGHTNESS 150

void setup() {

  Serial.begin(115200);

  HUB75_I2S_CFG mxconfig;
  mxconfig.mx_height = PANEL_HEIGHT;      // we have 64 pix height panels
  mxconfig.gpio.e = 18;

  // May or may not be needed depending on your matrix
  //mxconfig.clkphase = false;

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);

  dma_display->begin();

  dma_display->clearScreen();
  dma_display->fillScreen(myBLACK);
  dma_display->setTextWrap(false);

  dma_display->setTextSize(1);     // size 1 == 8 pixels high
  dma_display->setTextColor(myBLUE);
  dma_display->setCursor(0, 0);
  dma_display->print("Hello");

  dma_display->setTextSize(2);     // size 2 == 16 pixels high
  dma_display->setTextColor(myGREEN);
  dma_display->setCursor(0, 8);
  dma_display->print("Hello");

  dma_display->setTextSize(3);     // size 3 == 24 pixels high
  dma_display->setTextColor(myRED);
  dma_display->setCursor(0, 24);
  dma_display->print("Hello");

}

void loop() {
  unsigned long now = millis();
  if (now > ldrCheckDue) {
    ldrCheckDue = now + ldrDelay;
    int sensorValue = analogRead(LDR_PIN);
    Serial.print("Analog Read: ");
    Serial.println(sensorValue);
    int brightnessValue = map(sensorValue, MIN_INPUT, MAX_INPUT, MIN_DISPLAY_BRIGHTNESS, MAX_DISPLAY_BRIGHTNESS);
    dma_display->setBrightness8(brightnessValue);
  }

}
