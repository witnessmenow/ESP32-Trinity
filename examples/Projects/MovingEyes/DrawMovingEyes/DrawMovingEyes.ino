/*******************************************************************
    A sketch to draw eyes on a Matrix panel based on numbers recieved
    from the serial.

    This sketch needs to be flashed to the Trinity. The other sketch in
    this folder needs to be flashed to an ESP32-Cam board

    To connect the ESP32-Cam to the Trinity:
    ESP32Cam -> Trinity
    VCC -> 5V (I used the red screw terminal)
    GND -> GND (I used the black screw terminal)
    U0T (TX) -> RX (The center of the 3 pads just above the red and black terminals)

    Parts Used:
      ESP32 Trinity - https://github.com/witnessmenow/ESP32-Trinity
      ESP32-Cam* - https://s.click.aliexpress.com/e/_AkDuvD

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/

    * = Affilate

    Written by Brian Lough
      YouTube: https://www.youtube.com/brianlough
      Tindie: https://www.tindie.com/stores/brianlough/
      Twitter: https://twitter.com/witnessmenow
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

// -------------------------------------
// -------   Matrix Config   ------
// -------------------------------------

const int panelResX = 64;      // Number of pixels wide of each INDIVIDUAL panel module.
const int panelResY = 64;     // Number of pixels tall of each INDIVIDUAL panel module.
const int panel_chain = 1;      // Total number of panels chained one to another

// See the "displaySetup" method for more display config options

//------------------------------------------------------------------------------------------------------------------

MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);
uint16_t mySomething = dma_display->color565(0, 255, 255);

unsigned long coolDownTime = 0;
int lastLocation;

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

  // Can be set between 0 and 255
  // WARNING: The birghter it is, the more power it uses
  // Could take up to 3A on full brightness
  dma_display->setBrightness8(100); //0-255


  dma_display->clearScreen();
  lastLocation = 4; // 4 is the middle
  drawEye(lastLocation);

}


void drawEye(int location) {
  int offset = 4 - location;
  Serial.println(offset);
  dma_display->fillScreen(myBLACK);
  dma_display->drawCircle(panelResX / 4, panelResY / 2, 14, myRED);
  dma_display->fillCircle((panelResX / 4) + offset, panelResY / 2, 8, myRED);
  dma_display->drawCircle((panelResX / 4) * 3, panelResY / 2, 14, myRED);
  dma_display->fillCircle(((panelResX / 4) * 3) + offset, panelResY / 2, 8, myRED);
}

void getSerial() {
  int location;

  while (Serial.available() > 0) {
    location = Serial.parseInt();
    if (millis() > coolDownTime && location != lastLocation) {
      Serial.println(location);
      drawEye(location);
      lastLocation = location;
      coolDownTime = millis() + 200;
    }
  }

}


void loop() {
  getSerial();
}
