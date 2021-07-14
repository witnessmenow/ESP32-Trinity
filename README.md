# ESP32-Trinity

**This board will be open source when finished, but it's not finalized yet**

Info, examples, instructions and hardware files for the ESP32 Trinity, an open source ESP32 board for controlling HUB75 matrix panels.

## Help Support what I do!

My main contribution to the open source community to date has been my Arduino libraries for the ESP32 and ESP8266, but I recently been trying to contribute more certified open source hardware projects.

[If you enjoy my work, please consider becoming a Github sponsor!](https://github.com/sponsors/witnessmenow/)

## Aditional Hardware Required

To use the Trinity you will need the following additional things

- RGB LED Matrix. Please note that I can not gaurantee the following displays will defintly work with the library/board, They are ones I have bought in the past and work, but sellers have changed stock before.
    - 64x32 P3 Matrix display - [Aliexpress\*](https://s.click.aliexpress.com/e/_dYz5DLt)
    - 64x64 P3 Matrix display - [Aliexpress\*](https://s.click.aliexpress.com/e/_BfjY0wfp)
- A power supply, you can either use a barrel jack or USB-C (USB-C is limited to ~1.5A by a poly fuse)
    - [5V 8A laptop style power supply\*]( https://s.click.aliexpress.com/e/_d7uVLXt) - How much power the display uses varies based on how many LEDs are on, but this should cover you for anything you want to draw.
    - Any USB-C Power supply

* = affiliate link

## Setup and Configuration options

#### Setup

- On most displays, there are two connectors. There are arrows on the PCB of the display, the connector that the arrows are moving away from is the input. Insert the Trinity into this connector (there are arrows on the Trinity that should match the direction of the arrows on the display's PCB)
- Most display's come with a power cable, connect that to the display and also to the green screw terminals of the shield. (Red = 5V, black = GND)

#### Powering the board

- **The board and display take only 5V!**. 
- The recomended way of powering the board is to one of the following:
     - The 2.1mm barel jack
     - The USB-C port 
     - The red and black screw terminals. 
- The amount of amps needed depends on how many LEDs your project uses, but I recomend roughly 3A for a 64x32 display to cover all scenarios. Double it for a 64x64. I would also recomend allowing some head room, expecially if you are getting a cheaper supply.
- The USB-C connector is limited by a 1.5Amp poly fuse to ensure compatiblity with lower power USB supplies. It is possible to bypass this fuse.

#### Power Configuration

There is a 3pin connector with a jumper pin on the Trinity, this is for configuring the power setup. **Please note: "Diode Bypass" is not recomended when programming programming the Trinity, as your PC's USB port might not be able to provide the required current for the display.**

- **Jumper on the two pins nearest the USB connector, labeled "diode bypass"** - This allows you to power the matrix display using the USB-C connector.
- **Any other jumper config (including removing)** - The barel jack/screw terminals will power your display and provide power your ESP32. USB can be connected for programming, but will not power the matrix. 

## Software Setup

#### Board definition

You will need to have the ESP32 setup for your Arduino IDE, [instructions can be found here](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md). 

#### Library dependancies
- [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA) - the library the Trinity is designed to be used with. This is what interacts with the matrix panel. You can install this from the Arduino Library manager by searching for "ESP32 HUB75 MATRIX"
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) - is a dependacy of the matrix library. This can also be installed from the library manager by searching for "Adafruit GFX"
- [FastLED](https://github.com/FastLED/FastLED) - It is not a dependancy of the matrix library, but some of the cooler examples are built using the FastLED library, so it is worth installing that too. Again you can use the library manager and search for "FastLED".

#### Basic Usage

This board is designed to use the default pins of the [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA) where possible

*TODO*

#### Using a 64x64 display

*TODO*

#### Examples

*TODO*
