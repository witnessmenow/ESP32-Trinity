/*******************************************************************
    ESP32 as a bluetooth speaker with a Spectrum analyser display on the Matrix

    Heavily Based on https://github.com/thingpulse/esp32-icon64-a2dp

    *This is sill a work in progress*

    Modifications from the original sketch:
    - Updated to the most recent version of the bluetooth audio lib
    - Using the HUB75 Matrix display (plus the pin changes needed)
    - The graphics are no longer the original
    
    For use with the 
    - ESP32 Trinity
    - I2S Matrix Shield
    - Any ESP32 using the default wiring of the I2S matrix library (plus E = 18)

    Parts Used:
    ESP32 Trinity - https://github.com/witnessmenow/ESP32-Trinity#where-to-buy
    Adafruit Max 98357A I2S Amp

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/

    Adapted by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

 
// Original Codes header:

/*
  MIT License

  Copyright (c) 2020 ThingPulse GmbH (B.L.: Do I leave this here?)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------


#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA

#include "BluetoothA2DPSink.h"
// Library to make the ESP32 act as a Bluetooth speaker

// Think it needs to be installed through Github
// https://github.com/pschatzmann/ESP32-A2DP

#include <arduinoFFT.h>
// Library to do the FFT (seperating the audio into different freuencies) 

// Can be installed through the library manager
// https://github.com/kosme/arduinoFFT

// ----------------------------
// Dependency Libraries - each one of these will need to be installed.
// ----------------------------

// Adafruit GFX library is a dependancy for the matrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

// -------------------------------------
// -------   Matrix Config   ------
// -------------------------------------

const int panelResX = 64;      // Number of pixels wide of each INDIVIDUAL panel module.
const int panelResY = 64;     // Number of pixels tall of each INDIVIDUAL panel module.
const int panel_chain = 1;      // Total number of panels chained one to another

// See the "displaySetup" method for more display config options

// -------------------------------------
// -------   I2S Audio Config   ------
// -------------------------------------

#define I2S_DOUT      32
#define I2S_BCLK      22
#define I2S_LRC       21

// -------------------------------------
// -------   FFT Settings   ------
// -------------------------------------

#define NUM_BANDS  8
#define SAMPLES 512
#define SAMPLING_FREQUENCY 44100

// -----------------------------

MatrixPanel_I2S_DMA *dma_display = nullptr;

BluetoothA2DPSink a2dp_sink;

arduinoFFT FFT = arduinoFFT();

int32_t peak[] = {0, 0, 0, 0, 0, 0, 0, 0};
double vReal[SAMPLES];
double vImag[SAMPLES];

QueueHandle_t queue;

int16_t sample_l_int;
int16_t sample_r_int;

float amplitude = 200.0;

uint32_t animationCounter = 0;

int visualizationCounter = 0;
int32_t lastVisualizationUpdate = 0;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

int barWidth = 5;

void createBands(int i, int dsize) {
  uint8_t band = 0;
  if (i <= 2) {
    band =  0; // 125Hz
  } else if (i <= 5) {
    band =   1; // 250Hz
  } else if (i <= 7)  {
    band =  2; // 500Hz
  } else if (i <= 15) {
    band =  3; // 1000Hz
  } else if (i <= 30) {
    band =  4; // 2000Hz
  } else if (i <= 53) {
    band =  5; // 4000Hz
  } else if (i <= 106) {
    band =  6;// 8000Hz
  } else {
    band = 7;
  }
  int dmax = amplitude;
  if (dsize > dmax)
    dsize = dmax;
  if (dsize > peak[band])
  {
    peak[band] = dsize;
  }
}

void renderFFT(void * parameter) {
  int item = 0;
  for (;;) {
    if (uxQueueMessagesWaiting(queue) > 0) {

      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

      for (uint8_t band = 0; band < NUM_BANDS; band++) {
        peak[band] = 0;
      }

      for (int i = 2; i < (SAMPLES / 2); i++) { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
        if (vReal[i] > 2000) { // Add a crude noise filter, 10 x amplitude or more
          createBands(i, (int)vReal[i] / amplitude);
        }
      }

      // Release handle
      xQueueReceive(queue, &item, 0);

      uint8_t intensity;

      //      FastLED.clear();
      //      FastLED.setBrightness(BRIGHTNESS);
      dma_display->fillScreen(myBLACK);
      for (byte band = 0; band < NUM_BANDS; band++) {
        int barHeight = map(peak[band], 1, amplitude, 0, 60);
        //Serial.print(peak[band]);
        //Serial.print(","); //200000000
        //Serial.println(barHeight);

        //2 is the spacing between bars
        int x = 2 + (band * (barWidth + 2));
        //30

        if (barHeight == 0) {
          barHeight = 1;
        }
        dma_display->fillRect(x, 63 - barHeight, barWidth, barHeight, myGREEN);
      }
      dma_display->flipDMABuffer();

      //Serial.println("");

//      if ((millis() - lastVisualizationUpdate) > 1000) {
//        log_e("Fps: %f", visualizationCounter / ((millis() - lastVisualizationUpdate) / 1000.0));
//        visualizationCounter = 0;
//        lastVisualizationUpdate = millis();
//        //hueOffset += 5;
//      }
//      visualizationCounter++;
    }
  }
}

void avrc_metadata_callback(uint8_t data1, const uint8_t *data2) {
  Serial.printf("AVRC metadata rsp: attribute id 0x%x, %s\n", data1, data2);
}

// Then somewhere in your sketch:
void read_data_stream(const uint8_t *data, uint32_t length)
{
  int item = 0;
  // Only prepare new samples if the queue is empty
  if (uxQueueMessagesWaiting(queue) == 0) {
    //log_e("Queue is empty, adding new item");
    int byteOffset = 0;
    for (int i = 0; i < SAMPLES; i++) {
      sample_l_int = (int16_t)(((*(data + byteOffset + 1) << 8) | *(data + byteOffset)));
      sample_r_int = (int16_t)(((*(data + byteOffset + 3) << 8) | *(data + byteOffset + 2)));
      vReal[i] = (sample_l_int + sample_r_int) / 2.0f;;
      vImag[i] = 0;
      byteOffset = byteOffset + 4;
    }

    // Tell the task in core 1 that the processing can start
    xQueueSend(queue, &item, portMAX_DELAY);
  }
}

void displaySetup() {
  HUB75_I2S_CFG mxconfig(
    panelResX,   // module width
    panelResY,   // module height
    panel_chain    // Chain length
  );

  mxconfig.double_buff = true;

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

  pinMode(5, OUTPUT);

  // The queue is used for communication between A2DP callback and the FFT processor
  queue = xQueueCreate( 1, sizeof( int ) );
  if (queue == NULL) {
    Serial.println("Error creating the queue");
  }

  xTaskCreatePinnedToCore(
    renderFFT,          // Function that should be called
    "FFT Renderer",     // Name of the task (for debugging)
    10000,              // Stack size (bytes)
    NULL,               // Parameter to pass
    1,                  // Task priority
    NULL,               // Task handle
    1                   // Core you want to run the task on (0 or 1)
  );

  i2s_pin_config_t my_pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  a2dp_sink.set_i2s_port((i2s_port_t) 1);
  a2dp_sink.set_pin_config(my_pin_config);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
  a2dp_sink.set_stream_reader(read_data_stream);
  a2dp_sink.start("MyMusic");
}

void loop() {
}
