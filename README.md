# ESP32-i2s-Matrix-Shield
Info, instructions and examples for the ESP32 i2s Matrix Shield available on Tindie



## Software Setup

### Board definition

You will need to have the ESP32 setup for your Arduino IDE, [instructions can be found here](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md).

### Library dependancies
- [ESP32-RGB64x32MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA) - the library this shield is designed to be used with. This is what interacts with the matrix panel. You can install this from the Arduino Library manager by searching for "ESP32 64x32 LED MATRIX"
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) - is a dependacy of the matrix library. This can also be installed from the library manager by searching for "Adafruit GFX"
- [FastLED](https://github.com/FastLED/FastLED) - It is not a dependancy of the matrix library, but some of the cooler examples are built using the FastLED library, so it is worth installing that too. Again you can use the library manager and search for "FastLED".

### Basic Usage

This board is designed to use the default pins of the [ESP32-RGB64x32MatrixPanel-I2S-DMA library](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA) where possible, so you can use `dma_display.begin();` instead of passing in the custom pins.

#### Using a 64x64 display

If you are using a 64x64 display you will need to edit the [ESP32-RGB64x32MatrixPanel-I2S-DMA.h file](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA/blob/master/ESP32-RGB64x32MatrixPanel-I2S-DMA.h) that comes with the library. There are two changes required:
- Change `#define MATRIX_HEIGHT               32` to be **64**. It should be possible to do this from the Arduino sketch by defining it before you import the library, but I have not found this reliable at all.
- Change `#define E_PIN_DEFAULT   -1` to be **18**.
