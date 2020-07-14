# ESP32-i2s-Matrix-Shield
Info, instructions and examples for the [ESP32 i2s Matrix Shield available on Tindie](https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/)

![img](https://cdn.tindiemedia.com/images/resize/0KEMu_q8ZZl2meIWinbTlUCxH8M=/p/fit-in/1370x912/filters:fill(fff)/i/528221/products/2020-07-11T16%3A30%3A52.947Z-IMG_20200711_151941.jpg)

## Aditional Hardware Required

To use this shield you will need the following additional things

- [ESP32 Mini32 Development board (ESP32 D1 Mini)\*]( https://s.click.aliexpress.com/e/_dSi824B)
- RGB LED Matrix. Please note that I can not gaurantee the following displays will defintly work with the library/board, They are ones I have bought in the past and work, but sellers have changed stock before.
    - 64x32 P3 Matrix display - [Aliexpress\*](https://s.click.aliexpress.com/e/_dYz5DLt)
- [5V 8A laptop style power supply\*]( https://s.click.aliexpress.com/e/_d7uVLXt) - How much power the display uses varies based on how many LEDs are on, but this should cover you for anything you want to draw.

* = affiliate link

## Assembly Instructions

I created a video of me soldering up a board to use as instructions - [Link](https://www.youtube.com/watch?v=ZiR93TmSyE0&feature=youtu.be)

**TL;DW (aka steps for Bitluni)**
- All components sit on the top of the board other than the P-in connector and 3no. 1K resistors for C,D and E connections.
- **NOTE:** As the P-in connector and 1k resistors are underneath the ESP32 board, make sure that you solder them before blocking those pads. This is essential if you choose to solder the ESP32 directly to the board, but makes it easier even if you choose to use the header stand-offs
- Ground is a plane across the whole board, so you may need to increase the heat of your iron to solder Ground pads.

## Setup and Configuration options

#### Setup

- On most displays, there are two connectors. There are arrows on the PCB of the display, the connector that the arrows are moving away from is the input. Insert the shield into this connector (there are arrows on the shield that should match the direction of the arrows on the display's PCB)
- When inserting into this connector, make sure that it is inserted into the center of the connector as it is possible to insert it offset to one side.
- Most display's come with a power cable, connect that to the display and also to the green screw terminals of the shield. (Red = 5V, black = GND)

#### Powering the board

- **The board and display take only 5V!**. 
- The recomended way of powering the board is either using the 2.1mm barel jack or using the add-on screw terminals. 
- The amount of amps needed depends on how many LEDs your project uses, but I recomend roughly 4A for a 64x32 display to cover all scenarios. Double it for a 64x64.
- It is possible to power the display through the micro-usb of the ESP32, but I would be very cautious about doing this, as it is probably not designed to hangdle high amounts of current.

#### Power Configuration

There is a 3pin connector with a jumper pin on the shield, this is for configuring the power setup:

- **Jumper on the bottom two pins** - This should be the position you use unless you want to do something else specific. This mode allows power from your barel jack/screw terminals to power your ESP32, but will block the ESP32 from powering the display if you only have USB connected.
- **Jumper on the top two pins "diode bypass"** - This allows you to power the display using the USB power of the ESP32. I would be very cautious about doing this, as it is probably not designed to hangdle high amounts of current.
- **Removing Jumper** - If you want to power the ESP32 and the display seperately you can, removing the jumpers disconnects the 5V from the shield from the ESP32 5V pin. GND remains connected.

## Software Setup

#### Board definition

You will need to have the ESP32 setup for your Arduino IDE, [instructions can be found here](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md).

#### Library dependancies
- [ESP32-RGB64x32MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA) - the library this shield is designed to be used with. This is what interacts with the matrix panel. You can install this from the Arduino Library manager by searching for "ESP32 64x32 LED MATRIX"
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) - is a dependacy of the matrix library. This can also be installed from the library manager by searching for "Adafruit GFX"
- [FastLED](https://github.com/FastLED/FastLED) - It is not a dependancy of the matrix library, but some of the cooler examples are built using the FastLED library, so it is worth installing that too. Again you can use the library manager and search for "FastLED".

#### Basic Usage

This board is designed to use the default pins of the [ESP32-RGB64x32MatrixPanel-I2S-DMA library](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA) where possible, so you can use `dma_display.begin();` instead of passing in the custom pins.

#### Using a 64x64 display

If you are using a 64x64 display you will need to edit the [ESP32-RGB64x32MatrixPanel-I2S-DMA.h file](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA/blob/master/ESP32-RGB64x32MatrixPanel-I2S-DMA.h) that comes with the library. There are two changes required:
- Change `#define MATRIX_HEIGHT               32` to be **64**. It should be possible to do this from the Arduino sketch by defining it before you import the library, but I have not found this reliable at all.
- Change `#define E_PIN_DEFAULT   -1` to be **18**.

#### Examples

[ESP32-RGB64x32MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA) comes with a few examples that should help get you started.

- [Test Shapes](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA/blob/master/examples/testshapes_32x64/testshapes_32x64.ino) - This is a fairly basic example that will draw some shapes onto the dsiplay then show some text. This is a good example of seeing how to do basics with the display.
- Pattern Plasma - [Code](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA/tree/master/examples/PatternPlasma) - This is a really nice looking pattern that really shows off what the display can do

[Click here for larger](https://gfycat.com/nearajarermine-electronics-arduino-esp32)

![Pattern Plasma](https://thumbs.gfycat.com/NearAjarErmine-small.gif "Pattern Plasma")

- Aurora Demo - [Code](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA/blob/master/examples/AuroraDemo/AuroraDemo.ino) - A range of different patterns displayed on screen. Some of them are really cool. The example needs to slightly modified to work as by deafult it uses custom pins. Use `dma_display.begin();` and it should work

[Click here for larger](https://gfycat.com/blankfancyballoonfish)

![Aurora](https://thumbs.gfycat.com/BlankFancyBalloonfish-small.gif "Aurora")
