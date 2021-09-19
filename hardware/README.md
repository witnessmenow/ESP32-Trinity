# ESP32 Trinity Hardware

This folder contains the design files for the ESP32 Trinity and it's add-on boards.

## ESP32 Trinity

This is for the ESP32 Trinity itself. The schematic, gerber and BOM files for the V1.2 release of the board can be found in the [here](ESP32Trinity/V1.2).

This project was designed using EasyEDA and the entire project is shared here at [this link](https://oshwlab.com/ecenuig/esp32-trinity).


### Additional License info:

While this project is open source at the license mentioned below, the released version of the board makes use of two images that are licensed from thenounproject.com. These are non functional so I have also provided a gerber with these images removed (the one with "RoyaltyFree" at the end of the file name)

If you use the released Trinity design you will need to sort out licensing for these images yourself ($2.99 each covers a license for commercial use)

- [Triquetra by Rflor](https://thenounproject.com/icon/3612617/)
- [Finderprint by Kevin White, US](https://thenounproject.com/icon/584207/)


## Add-On Boards

These are a collection of designs I made for the Trinity. These slot into the add-on/GY521 slot on the Trinity.

- Wii Nunchuck Board - For connecting Nintendo Wii Nunchuck controller to your Trinity. They use I2C to communicate. [EasyEDA Project](https://oshwlab.com/ecenuig/nunchuck-mpu6050)
- Micro SD Card Board - A design for connecting micro SD cards to your trinity. [EasyEDA Project](https://oshwlab.com/ecenuig/sd-card-gy521)
- Add-on Breakout - A design for breaking out the pins of the add on port, useful if you don't want to be able to swap out what is connected to the Add-on port. Includes two QWIIC connectors (that I have not tested as I do not have any QWICC devices here). [EasyEDA Project](https://oshwlab.com/ecenuig/trinity-add-on-breakout)


## License

All hardware contained in this project is licensed under the CERN Open Hardware License V1.2. Details of the license can be found in license.md at the root of the repo. 