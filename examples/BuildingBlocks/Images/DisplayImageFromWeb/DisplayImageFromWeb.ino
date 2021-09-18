/*******************************************************************
    Display an image loaded from the internt on a matrix display

    This is a more generic version of the functionality worked on
    for the Spotify library.

    If you want to incorporate this into a larger project, I would
    suggest taking a look at it's implentation in the spotify library
    as the fetching of the image does not rely on the httpClient,
    but im not sure sure if thats is a big advantage or not!

    The image loaded is a 64 x 64 image, but it should display on smaller
    matrix (just be cut off). 
    
    The library for the display will need to be modified to work
    with a 64x64 matrix:
    https://github.com/witnessmenow/ESP32-i2s-Matrix-Shield#using-a-64x64-display 

    This example is currently set up for HTTPS image sources, and
    probably needs to be changed if the source just uses HTTP
    (Mainly if you just replace Web_Fetch.h with the code from
     the example that comes with the TJpg library)

    This is a mashup of two examples from the TJpg_Decoder library,
    further modified to add support for HTTPS

    Parts:
    ESP32 D1 Mini * - https://s.click.aliexpress.com/e/_dSi824B
    ESP32 I2S Matrix Shield (From my Tindie) = https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/

    More info on the shield: https://github.com/witnessmenow/ESP32-i2s-Matrix-Shield

 *  * = Affilate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

/* Original header of example */

// Example for library:
// https://github.com/Bodmer/TJpg_Decoder

// This example is for an ESP8266 or ESP32, it fetches a Jpeg file
// from the web and saves it in a SPIFFS file. You must have SPIFFS
// space allocated in the IDE.

// Chenge next 2 lines to suit your WiFi network

// ----------------------------
// Standard Libraries
// ----------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

#define FS_NO_GLOBALS
#include <FS.h>
#include "SPIFFS.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 64x32 LED MATRIX")
// https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA

#include <TJpg_Decoder.h>
// Library for decoding Jpegs from the API responses

// Search for "tjpg" in the Arduino Library manager
// https://github.com/Bodmer/TJpg_Decoder

// ----------------------------
// Include Other Tabs
// ----------------------------

#include "List_SPIFFS.h"
#include "Web_Fetch.h"

//------- Replace the following! ------

char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network password

// If you need to add a server cert for verifying the endpoint,
// Line 15 of web_fetch.h is where you would add it

//------- ---------------------- ------

// HTTPS image url
#define IMAGE_URL "https://i.imgur.com/1TBUEDE.jpg"

// Note the "/" at the start
#define IMAGE_LOCATION "/img.jpg"

RGB64x32MatrixPanel_I2S_DMA dma_display;

// This function will be called during decoding of the jpeg file to
// render each block to the Matrix.
bool displayOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  // Stop further decoding as image is running off bottom of screen
  if ( y >= dma_display.height() ) return 0;

  dma_display.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n Testing TJpg_Decoder library");

  // Initialise SPIFFS, if this fails try .begin(true)
  // NOTE: I believe this formats it though it will erase everything on
  // spiffs already! In this example that is not a problem.
  // I have found once I used the true flag once, I could use it
  // without the true flag after that.

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nInitialisation done.");

  dma_display.begin();
  dma_display.fillScreen(dma_display.color565(255, 0, 0));

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(displayOutput);

  // The byte order can be swapped (set true for TFT_eSPI)
  //TJpgDec.setSwapBytes(true);

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


  // This is for demoe purposes only so that file is fetched each time this is run
  if (SPIFFS.exists(IMAGE_LOCATION) == true) {
    Serial.println("For test only, removing file");
    SPIFFS.remove(IMAGE_LOCATION);
  }
}

void loop()
{
  // List files stored in SPIFFS
  listSPIFFS();

  // Time recorded for test purposes
  uint32_t t = millis();

  // Fetch the jpg file from the specified URL and save in given location
  bool loaded_ok = getFile(IMAGE_URL, IMAGE_LOCATION);

  t = millis() - t;
  if (loaded_ok) {
    Serial.print(t);
    Serial.println(" ms to download");
  }

  // List files stored in SPIFFS, should have the file now
  listSPIFFS();

  t = millis();

  // Now draw the SPIFFS file
  TJpgDec.drawFsJpg(0, 0, IMAGE_LOCATION);

  t = millis() - t;
  Serial.print(t); Serial.println(" ms to draw to Matrix");

  // Wait forever
  while (1) yield();
}
