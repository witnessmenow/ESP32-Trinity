/*******************************************************************
    Display an image loaded from the internt on a matrix display

    This is a more generic version of the functionality worked on
    for the Spotify library.

    This loads an image from imgur. It uses https, but it could be adapted to
    http by just using a WiFiClient, rahter than WiFiClientSecure

    The image loaded is a 64 x 64 image, but it should display on smaller
    matrix (just be cut off).

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
// Standard Libraries
// ----------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <FS.h>
#include "SPIFFS.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA

#include <JPEGDEC.h>
// Library for decoding Jpegs from the API responses

// Search for "JPEGDEC" in the Arduino Library manager
// https://github.com/bitbank2/JPEGDEC/

// ----------------------------
// Dependency Libraries - each one of these will need to be installed.
// ----------------------------

// Adafruit GFX library is a dependency for the matrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

// ----------------------------
// Include Other Tabs
// ----------------------------

#include "WebRequest.h"

// -------------------------------------
// -------   Matrix Config   ------
// -------------------------------------

const int panelResX = 64;      // Number of pixels wide of each INDIVIDUAL panel module.
const int panelResY = 64;     // Number of pixels tall of each INDIVIDUAL panel module.
const int panel_chain = 1;      // Total number of panels chained one to another

// See the "displaySetup" method for more display config options

//------------------------------------------------------------------------------------------------------------------

// -------------------------------------
//------- Other Config - Replace the following! ------
// -------------------------------------

char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network password

// If you need to add a server cert for verifying the endpoint,
// Line 15 of web_fetch.h is where you would add it

//------- ---------------------- ------

// HTTPS image url
const char imageUrl[] = "https://i.imgur.com/1TBUEDE.jpg";

// Note the "/" at the start
const char imageLocation[] = "/img.jpg";

MatrixPanel_I2S_DMA *dma_display = nullptr;
JPEGDEC jpeg;
WiFiClientSecure client;

// This next function will be called during decoding of the jpeg file to
// render each block to the Matrix.  If you use a different display
// you will need to adapt this function to suit.
int JPEGDraw(JPEGDRAW *pDraw)
{
  // Stop further decoding as image is running off bottom of screen
  if (  pDraw->y >= dma_display->height() ) return 0;

  dma_display->drawRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);

  // Return 1 to decode next block
  return 1;
}

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

void setup()
{
  Serial.begin(115200);

  displaySetup();

  dma_display->fillScreen(dma_display->color565(0, 0, 0));

  dma_display->setTextSize(1);     // size 1 == 8 pixels high
  dma_display->setTextWrap(false); // Don't wrap at end of line - will do ourselves
  dma_display->setTextColor(dma_display->color565(255, 0, 255));

  dma_display->setCursor(1, 0);    // start at top left, with 8 pixel of spacing

  dma_display->print("SPIFFS: ");
  dma_display->setCursor(48, 0);
  // Initialise SPIFFS, if this fails try .begin(true)
  if (!SPIFFS.begin() && !SPIFFS.begin(true)) {
    dma_display->print("NO!");
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  } else {
    dma_display->print("OK!");
  }

  dma_display->setCursor(1, 10);    // start at top left, with 8 pixel of spacing
  dma_display->print("WiFi: ");
  dma_display->setCursor(48, 10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dma_display->print("OK!");

  // this line ignores the HTTPS validation check
  // You could set a cert here instead to check the server
  // you are talking to is who you expect
  client.setInsecure();

  // Makes the example a little simpler to have a global for it in WebRequest
  webClient = &client;

}

fs::File myfile;

// Callbacks used by the jpeg library

void * myOpen(const char *filename, int32_t *size) {
  myfile = SPIFFS.open(filename);
  *size = myfile.size();
  return &myfile;
}
void myClose(void *handle) {
  if (myfile) myfile.close();
}
int32_t myRead(JPEGFILE *handle, uint8_t *buffer, int32_t length) {
  if (!myfile) return 0;
  return myfile.read(buffer, length);
}
int32_t mySeek(JPEGFILE *handle, int32_t position) {
  if (!myfile) return 0;
  return myfile.seek(position);
}

void drawImageFile(char *imageFileUri) {
  unsigned long lTime = millis();
  lTime = millis();
  jpeg.open(imageFileUri, myOpen, myClose, myRead, mySeek, JPEGDraw);
  jpeg.decode(0, 0, 0);
  jpeg.close();
  Serial.print("Time taken to decode and display Image (ms): ");
  Serial.println(millis() - lTime);
}

int displayImage(const char* image) {

  // In this example I re-use the same filename
  // over and over, but you could check does the image
  // exist and not fetch it again
  if (SPIFFS.exists(imageLocation) == true) {
    Serial.println("Removing existing image");
    SPIFFS.remove(imageLocation);
  }

  fs::File f = SPIFFS.open(imageLocation, "w+");
  if (!f) {
    Serial.println("file open failed");
    return -1;
  }

  unsigned long lTime = millis();

  bool gotImage = getImage((char *)image, &f);

  Serial.print("Time taken to get Image (ms): ");
  Serial.println(millis() - lTime);

  // Make sure to close the file!
  f.close();

  if (gotImage) {
    drawImageFile((char *)imageLocation);
    return 0;
  } else {
    return -2;
  }
}

void loop()
{

  // Fetch the jpg file from the specified URL and save in given location
  int displayImageResult = displayImage(imageUrl);

  if (displayImageResult == 0) {
    Serial.print("Image should be displayed! ");
  } else {
    Serial.print("Failed to display image: ");
    Serial.println(displayImageResult);
  }

  // Wait forever
  while (1) yield();
}
