### What is it?

The ESP32 Trinity is an open source ESP32 board for controlling RGB Matrix panels. It comes fully assembled and ready to plug into your matrix.

It is designed to work with the [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA) library.

NOTE: This board will only work with Matrix displays that the I2S library supports. Please check out the libraries' [Github page](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA#panels-supported) for more information.

### Why did you make it?

The RGB LED matrix panels are one of the coolest displays you can use with an Arduino, but there is quite a bit of wiring to connect them up and also they are a little awkward to power. 

The panels are physically quite large and are only about $20/$25 from aliexpress/eBay.

The Trinity makes wiring them really simple! The board just slots directly onto the input header pin and has screw terminals to connect the power cable that comes with most panels.


### What makes it special?

Soldering the I2S matrix shield was a barrier to entry for people that I wanted to remove by making a fully assembled board. I was also able to add additional features such as much better support for powering the project via USB.

This really makes using the display as simple as possible and can be used to make some pretty awesome looking projects! As it is fully assembled, you just plug it into your matrix, attach the power cables and you are ready to start making!

It also has all the available pins broken out so you can make use of it in your project. The add on area is designed for GY521 (MPU6050) accelerometer module so you can make some pretty interesting falling sand projects with it! But you can use these pins for other devices either.

### Examples sketches for using these displays.

I have a lot of examples and sample projects available on [the projects Github page](https://github.com/witnessmenow/ESP32-Trinity/tree/master/examples) for you to get started with.

The [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA) library also comes with some pretty cool examples. I really recommend checking out PatternPlasma and AuroraDemo

Note: These displays are difficult to video correctly, they look much better in person!

#### PatternPlasma- [Click here for larger](https://gfycat.com/nearajarermine-electronics-arduino-esp32)

![Pattern Plasma](https://thumbs.gfycat.com/NearAjarErmine-small.gif "Pattern Plasma")

#### Aurora - [Click here for larger](https://gfycat.com/blankfancyballoonfish)

![Aurora](https://thumbs.gfycat.com/BlankFancyBalloonfish-small.gif "Aurora")

### What is included?

Each ESP32 Trinity comes assembled with the following additional components:

- An LDR - The reason its not assembled is you may want to place it on the top of your panel
- 8p Female header for the Breakout area
- 16p (2x8) Stacking header (to be used if the Trinity does not fit with your matrix panel)

### Additional Hardware Required

To use the Trinity you will need the following additional things

- Compatible HUB75/HUB75E RGB LED Matrix Panel
- Most (all?) panels come with a power cable, this board requires this.
- A 5V PSU cable of running the display. Please checkout [the Github page](https://github.com/witnessmenow/ESP32-Trinity/blob/master/setup.md#powering-the-project) for more info

### Will it work with my display?

Honestly, I can't answer that for sure, all I can say is it should work with any display that works with the [ESP32-HUB75-MatrixPanel-I2S-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA). I can't guarantee that all display's will work.

The Trinity has mainly been tested on 64x32 and 64x64 Matrix panels.

Even links to displays I have bought in past that work are subject to new stock/revisions (this happened previously).

### More information

Please checkout [the ESP32 Trinity Github page](https://github.com/witnessmenow/ESP32-Trinity) for
- Coding Examples and Sample projects
- Setup Instructions
- Hardware files and schematics

### Coding Support

Please note that I can not provide coding support for this item. I can offer some troubleshooting advice on getting them working (but please also read the section labeled "will this work with my display") and I can point you to some examples I have created using these board, but I will not be able to give support on building something specific.

### Change-log of versions

- V1.2 - First Released version