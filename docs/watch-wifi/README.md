# ESP32 Watch Wi-Fi Scan Test

This test checks that the Waveshare ESP32-S3-Touch-AMOLED-2.06 can scan for nearby Wi-Fi networks and show the results on its AMOLED screen.

## What the code does

1. Starts the AMOLED display.
2. Puts the ESP32-S3 Wi-Fi radio in station mode.
3. Shows a scanning screen while it searches for nearby networks, including hidden networks.
4. Displays up to seven networks per page.
5. Automatically changes pages every five seconds and starts a new scan after the last page.
6. Prints the same scan details to the USB Serial Monitor.

![Watch scanning for Wi-Fi](images/wifi-scanning-screen.png)

![Wi-Fi scan results displayed on the watch](images/wifi-scan-results.png)

## Understanding the output

- `Found` is the total number of networks detected during the scan.
- `Page` shows the current results page and the total number of pages.
- The network name is the SSID. A network that does not broadcast its name appears as `<hidden>`.
- `dBm` is received signal strength. A value closer to 0 is stronger. The display uses green for strong signals, yellow for medium signals, and red for weak signals.
- `CH` is the 2.4 GHz Wi-Fi channel used by the network.

## Wi-Fi limitation

The ESP32-S3 radio in this watch supports 2.4 GHz Wi-Fi only. It cannot detect or connect to a 5 GHz-only network. A dual-band router or phone hotspot must have its 2.4 GHz band enabled for the watch to find it.

## Libraries and files

- `Arduino.h` provides the standard Arduino functions and types.
- `WiFi.h` comes with the Espressif ESP32 Arduino board package and performs the network scan.
- `Arduino_GFX_Library.h` is the Waveshare-provided Arduino_GFX library used to drive the CO5300 AMOLED display.
- `HWCDC.h` comes with the ESP32 Arduino board package and provides USB serial output.
- `pin_config.h` is a local board header containing the display pin numbers, screen size, and other board pin definitions. It is not an Arduino Library Manager package.

## File structure

```text
wifi-connection/
|-- README.md
|-- images/
|   |-- wifi-scan-results.png
|   `-- wifi-scanning-screen.png
`-- src/
    |-- main.cpp
    `-- pin_config.h
```

## Basic Arduino IDE use

1. Install the Espressif ESP32 board package in Arduino IDE.
2. Install the Waveshare version of Arduino_GFX supplied with the official ESP32-S3-Touch-AMOLED-2.06 examples.
3. Create an Arduino sketch and copy the contents of `src/main.cpp` into the sketch's `.ino` file.
4. Copy `src/pin_config.h` into the same sketch folder.
5. Select `ESP32S3 Dev Module`, the watch's COM port, OPI PSRAM, and the correct flash settings for the board.
6. Upload the sketch. Open Serial Monitor at 115200 baud to see the full scan results.

The stored `src/main.cpp` name also makes the same source suitable for a normal `src`-based embedded project layout.
