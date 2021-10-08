# Ready To Flash Projects

These projects are ready to flash directly to your Trinity directly from your browser, you don't even need the Arduino IDE installed!

## Steps to flash

- You will need the [CH340 driver](https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all) installed.
- You will need to visit the flash pages using Chrome. (Other browsers don't support the WebSerial stuff yet)
- Plug your Trinity into your PCs USB port
- Click the "Install" button on the page and select your Trinity com port.


## WiFi Tetris Clock

My Wifi Tetris Clock project is a clock that gets it's time from the internet and draws the numbers out using falling tetris blocks!

![Animated image showing Wifi Tetris Clock project](https://content.instructables.com/ORIG/FS0/N0XF/KP41SN17/FS0N0XFKP41SN17.gif)

#### Required hardware

It is originally designed to run on a 64x32 matrix, but can be configured to run on a 64x64 in the config interface


#### After flashing

Once flashed, you will need to connect to the access point it hosts (details on the web flash page) and configure it. You will need to set the WiFi details of your network and you'll probably want to set your timezone too.

You can r-enter the config mode by pressing the reset button twice (about 1 second apart)

[Web Flash Link](https://witnessmenow.github.io/ESP32-Trintiy-Web-Flash/WifiTetrisClock/)
