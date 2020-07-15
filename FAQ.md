# Frequently Asked Questions

This section is a lie because as of the time of writing, nobody has asked any questions. So this section should probably be named PFAQ (Potential Frequently Asked Questions)

### Should I buy this version or the PxMatrix version?

Tough one to get us started! I think PxMatrix is a little easier to use in someways becasue you do not need to edit the header of the library but the I2S library is a bit more straight forward to use. PxMatrix has proven good support too, I'm less familar with the support levels of the I2S library. There are alot more examples at the moment that use PxMatrix.

But saying that there are some big advantages to this shield.
- It's smaller
- Does not need the ribbon cable to the out connector
- I2S library is faster - [Check out Aaron Christophel's FPS comparison!](https://www.youtube.com/watch?v=HKWDGangWU0)
- It has a better chance with working with more displays. PxMatrix supported multiple displays, but struggled to handle over 3. The I2S library has a better chance of working with more (I have not tested how many though)

Personally, for future projects I will more than likely use this as my go to. If you are a beginner, maybe PxMatrix is an easier choice due to all the examples.

### Will it work with my display?

Honestly, I can't answer that for sure, all I can say is it should work with any display that works with the [ESP32-RGB64x32MatrixPanel-I2S-DMA library](https://github.com/mrfaptastic/ESP32-RGB64x32MatrixPanel-I2S-DMA). I can't guarantee that all display's will work. Even links to displays I have bought in past that work are subject to new stock/revisions (this happened previously).

### What pins are used up and what pins are available?

[The schematic of the product can be found here](https://i.imgur.com/wbVGML8.png). 

Here is a list of pins that are used by the display:

```
#define R1_PIN_DEFAULT  25
#define G1_PIN_DEFAULT  26
#define B1_PIN_DEFAULT  27
#define R2_PIN_DEFAULT  14
#define G2_PIN_DEFAULT  12
#define B2_PIN_DEFAULT  13

#define A_PIN_DEFAULT   23
#define B_PIN_DEFAULT   19
#define C_PIN_DEFAULT   5
#define D_PIN_DEFAULT   17
#define E_PIN_DEFAULT   18 // This is the only change from the deafult pins of the library
          
#define LAT_PIN_DEFAULT 4
#define OE_PIN_DEFAULT  15

#define CLK_PIN_DEFAULT 16
```

As of V1.2, only two pin of the ESP32 are actually broken out anywhere, the two i2c pins which are broken out to the accelerometer pads. These are the default i2c pins for the ESP32 so you should just be able to use the Wire object without specifying pins

```
#define SDA 21
#define SCL 22
```

All other free pins of the ESP32 are technically available to use, but not really accessible as you would have to solder to the header pin. If you have a use case where you want more pins, please raise a github issue and I'll look into it!
