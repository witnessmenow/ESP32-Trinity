# ESP32-i2s-Matrix-Shield
Info, instructions and examples for the [ESP32 i2s Matrix Shield available on Tindie](https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/)

## Aditional Hardware Required

To use this shield you will need the following additional things

- [ESP32 Mini32 Development board (ESP32 D1 Mini)\*]( https://s.click.aliexpress.com/e/_dSi824B)
- RGB LED Matrix. Please note that I can not gaurantee the following displays will defintly work with the library/board, They are ones I have bought in the past and work, but sellers have changed stock before.
    - 64x32 P3 Matrix display - [Aliexpress\*](https://s.click.aliexpress.com/e/_dYz5DLt)
- [5V 8A laptop style power supply\*]( https://s.click.aliexpress.com/e/_d7uVLXt) - How much power the display uses varies based on how many LEDs are on, but this should cover you for anything you want to draw.

* = affiliate link

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

### Examples

[ESP32-RGB64x32MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA) comes with a few examples that should help get you started.

- [Test Shapes](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA/blob/master/examples/testshapes_32x64/testshapes_32x64.ino) - This is a fairly basic example that will draw some shapes onto the dsiplay then show some text. This is a good example of seeing how to do basics with the display.
- [Pattern Plasma](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA/tree/master/examples/PatternPlasma) - This is a really nice looking pattern that really shows off what the display can do
- [Aurora Demo](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA/blob/master/examples/AuroraDemo/AuroraDemo.ino) - A range of different patterns displayed on screen. Some of them are really cool. The example needs to slightly modified to work as by deafult it uses custom pins. Use `dma_display.begin();` and it should work
