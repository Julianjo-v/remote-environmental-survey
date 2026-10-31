# Research Photos: Version 1 ESP32 Watch Wi-Fi Scan Test

This directory stores research photos and documentation for Version 1 of the
watch Wi-Fi test. The basic code in
`src/watch/watch_wifi_scan_test.cpp` checks that the Waveshare
ESP32-S3-Touch-AMOLED-2.06 can scan for nearby Wi-Fi networks and show the
results on its AMOLED screen.

The photos below are evidence from this first version, before later
SquareLine Studio and LVGL user-interface work. The user reports that the later
graphical interface ran successfully on the physical watch with a touch switch
that started scanning and a scrollable, clickable network list. Those later
runtime artifacts and source files are not currently included in this
repository, so the photos here remain Version 1 evidence only.

## What the code does

1. Starts the AMOLED display.
2. Puts the ESP32-S3 Wi-Fi radio in station mode.
3. Shows a scanning screen while it searches for nearby networks, including hidden networks.
4. Displays up to seven networks per page.
5. Automatically changes pages every five seconds and starts a new scan after the last page.
6. Prints the same scan details to the USB Serial Monitor.

![Version 1 watch scanning for Wi-Fi](images/v1-wifi-scanning-screen.png)

![Version 1 Wi-Fi scan results displayed on the watch](images/v1-wifi-scan-results.png)

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
- `src/watch/watch_wifi_scan_test.cpp` contains the Version 1 scan test.
- `src/watch/pin_config.h` is a local board header containing the display pin numbers, screen size, and other board pin definitions. It is not an Arduino Library Manager package.

## File structure

```text
remote-environmental-survey/
|-- src/
|   `-- watch/
|       |-- watch_wifi_scan_test.cpp
|       `-- pin_config.h
`-- photos/
    |-- README.md
    `-- images/
        |-- v1-wifi-scan-results.png
        `-- v1-wifi-scanning-screen.png
```

## Basic Arduino IDE use

1. Install the Espressif ESP32 board package in Arduino IDE.
2. Install the Waveshare version of Arduino_GFX supplied with the official ESP32-S3-Touch-AMOLED-2.06 examples.
3. Create an Arduino sketch and copy the contents of `src/watch/watch_wifi_scan_test.cpp` into the sketch's `.ino` file.
4. Copy `src/watch/pin_config.h` into the same sketch folder.
5. Select `ESP32S3 Dev Module`, the watch's COM port, OPI PSRAM, and the correct flash settings for the board.
6. Upload the sketch. Open Serial Monitor at 115200 baud to see the full scan results.

The `src/watch/` directory keeps the Version 1 scan source and its board header
together for later project integration.
