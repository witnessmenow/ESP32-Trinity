## Examples

Here you will find example sketches to run on your Trinity ranging from a simple "Hello World" to full projects like the WiFi Tetris clock

These examples are compatible with any ESP32 using the I2S Matrix library (including my I2S Matrix Shield).

You should also check out the [examples that come with the I2S Matrix library](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/tree/master/examples) for more things to run, but they might not be configured to work with the Trinity out of the box

### Basics

In this section you will find some simple sketches to get you up and running on your Trinity. These are a great place to get started, as I alway would recomend getting something simple running on any hardware to make sure your setup works!

- [Hello World](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Basics/HelloWorld) - A simple example showing different size fonts in different colours
- [ColourTest](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Basics/ColourTest) - A simple sketch that just cycles through colours (WARNING - can use a lot of power!)
- [PanelChecker](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Basics/PanelChecker) - A modified version of the Hello World that can be used to change the matrix config by using the Trinity touch buttons on startup. Check the Usage at the top of the sketch.

### Building Blocks

This section shows examples on how to do some things that could be useful to use inside a different project. I've also tried to cover some of the more commonly asked questions of how to do things here too

- Text
  - [CenterText](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/BuildingBlocks/Text/CenterText) - An example for showing you how to center align text.
  - [ScrollingText](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/BuildingBlocks/Text/ScrollingText) - An example to show you how to scroll text.
- Images
  - [DisplayImageFromWeb](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/BuildingBlocks/Images/DisplayImageFromWeb) - Downloads a JPG from a URL (imgur in this case), save it to SPIFFS and displays it to the Matrix
  - [DisplayGIF](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/BuildingBlocks/Images/DisplayGIF) - Displays severtal GIFs on the matrix. Check the top of the file for more info.

### Trinity Features

This section is show how to use some of the additional features of the Trinity

- [LDRAutoBrightness](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/TrinityFeatures/LDRAutoBrightness) - An example of using the LDR to automatically adjust the brightness of the display **Additional Requirements:** Included LDR needs to be soldered
- [SDCardTest](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/TrinityFeatures/SDCardTest) - An example of using an SD Card with your Trinity. Could be adapted to work with any SPI device. The matrix uses some of the deafult SPI pins so we had to get creative. **Additional Requirements:** An SD/Micro SD module
- [TouchPointTest](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/TrinityFeatures/TouchPointTest) - A basic example of using the built-in Touch pads of the Trinity.

### Projects

This is a collection of some of the projects I've written for the Trinity that you should be able to just use to get a full working project going on your Trinity in just a few minutes! Also feel free to build on top of these or change them for you needs.

_Note:_ This is not a full list of projects I've made as some will have their own repos. Please check the Project Section for a full list of my projects and user contributed ones.

- [FallingSand](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Projects/FallingSand):
  A fun project that emulates sand falling on the Matrix using an accelerometer module. This was originally created by [Tetra3DPrint](https://twitter.com/tetra3dprint) and ported to the I2S matrix library by me.
  **Additional Requirements:** GY521 Accelermoter Module
- [SnakeWithNunchuck](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Projects/SnakeWithNunchuck)
  I'm tired of these monkey fighting snakes on this Monday to Friday Matrix! A simple version of the clasic game controlled by a Nintendo Wii Nunchuck. **Additional Requirements:** Wii Nunchuck and an adaptor
- [TetrisWithNunchuck](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Projects/TetrisWithNunchuck)
  Please don't sue me EA. Tetris on your matrix controlled by a Wii Nunchuck. **Additional Requirements:** Wii Nunchuck and an adaptor
- [WifiTetrisClock](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Projects/WifiTetrisClock)
  Really, Please don't sue me EA. A clock displayed using falling matrix blocks. [Video about it here](https://www.youtube.com/watch?v=ey2mjZ-UQNM) but it refers to the original project I ported this code from.
- [WifiTetrisClockWifiManager](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Projects/WifiTetrisClockWifiManager)
  The same as above, but uses WiFi manager to manage your Wifi details and other configs.
- [YouTubeLivestreamDraw](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Projects/YouTubeLivestreamDraw)
  Allow your livestream viewers chat draw on your matrix, what could go wrong? [See in action on Unexpected Makers channel](https://www.youtube.com/watch?v=qZgrMAGGyhM&t=5592s)
- [TrafficLight](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples/Projects/TrafficLight)
  A simple traffic light that changes based on time.

### But I still haven't found what I'm looking for

If you feel like there is something missing please or you just need some addtional help, feel free to:

- [Ask on my Discord](https://discord.gg/2enC6GW) - This will be the fastest as other helpful makers can jump in too!
- Raise a Github Issue here

And I will do my best to help if I can!
