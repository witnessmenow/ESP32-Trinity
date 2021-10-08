# Setup and Configuration options

This page will cover the basics of setting up the ESP32 Trinity

## Hardware Setup

### Connecting to the display

Note: The Trinity has mainly been tested using 64x32 and 64x64 matrix panels.

- On most displays, there are two connectors. There are arrows on the PCB of the display, the connector that the arrows are moving away from is the input. Insert the Trinity into this connector (there are arrows on the Trinity that should match the direction of the arrows on the display's PCB). The Trinity connector is keyed so it can only be plugged in the correct way (once you pick the right connector!)

![Picture showing the arrow on the display](/images/instructions/matrixArrow.jpg)

- Most display's come with a power cable, connect that to the display and also to the green screw terminals of the Trinity. (Red cable = 5V, black cable = GND)

![Picture showing the screw terminals](/images/instructions/screwTerminals.jpg)

### Powering the project

#### Power info

- **The Trinity and display take 5V only!**.
- The recommend way of powering the board is to one of the following:
  - The 2.1mm barrel jack
  - The USB-C port (will not power the matrix out of the box, please see the section below for details)
  - The red and black screw terminals.
- All methods of powering the board will also supply power to the ESP32.
- The amount of amps needed depends on how many LEDs your project uses and how bright you make them, but I recommend roughly 3A for a 64x32 display to cover all scenarios and maybe 4A for a 64x64. I would also recommend allowing some head room, especially if you are getting a cheaper supply.

#### Powering the matrix panel using USB-C

By default the Trinity stops the USB-C power from reaching the Matrix panel. This is because some lower Amperage USB sources, such as your PC's USB port, may not be able to provide the power required to drive the display.

If you are using a higher Amperage USB supply, such as a phone charger, you can enable powering the matrix via USB-C by moving the jumper pin to the two pins nearest the USB connector. (labeled "Diode Bypass")

![Picture showing the screw terminals](/images/instructions/screwTerminals.jpg)

Powering the display via USB-C is limited by a 1.5/3 Amp poly fuse to protect your USB power supply. This basically means the fuse may cut-off on current draws of 1.5A or higher, but will definitely cut off at draws of 3A (the max USB supports @ 5V). It is possible to bypass this fuse, but do so at your own risk.

Any other configuration for the jumper (including removing it) will return the default behavior of not allowing USB-C power to reach the Matrix panel.

**Please note: "Diode Bypass" should be removed when programming the Trinity, as you may damage your PC's USB port if the display tries to draw too much power.**

## Software Setup

### Driver

The Trinity uses the CH340 USB to UART chip. If you do not have a driver already installed for this chip you may need to install one. Check out [Sparkfun's guide for installation instruction](https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all)

### Board definition

You will need to have the ESP32 setup for your Arduino IDE, [instructions can be found here](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).

You can then select basically any ESP32 board in the boards menu. (I usually use "Wemos D1 Mini ESP32", but it doesn't really matter)

### Library despondencies

The following libraries will need to be installed for the Trinity to work.

- [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA) - the library the Trinity is designed to be used with. This is what interacts with the matrix panel. You can install this from the Arduino Library manager by searching for "ESP32 HUB75 MATRIX"
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library) - is a dependency of the matrix library. This can also be installed from the library manager by searching for "Adafruit GFX"
- [FastLED](https://github.com/FastLED/FastLED) - It is not a dependency of the matrix library, but some of the libraries cooler examples are built using the FastLED library, so it is worth installing that too. Again you can use the library manager and search for "FastLED".

### Hello World

I've provided a simple ["Hello World" example](/examples/Basics/HelloWorld) to get you started. This will cover the basics of setting your matrix panel for your project.

### Examples

I have provided lots of examples for you to try out to get some ideas or inspiration. [Check them out here.](/examples/)
