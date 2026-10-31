# Version 1: Basic Wi-Fi Scanner

This folder preserves the historical Version 1 watch prototype for the exact
Waveshare ESP32-S3-Touch-AMOLED-2.06. It scans nearby 2.4 GHz Wi-Fi networks
and displays SSID, RSSI, and channel information on the AMOLED screen in
automatically changing pages.

Version 1 remains unchanged as early measured subsystem evidence. The newer
touch-controlled interface is preserved separately in
[`../version-2-squareline-wifi/`](../version-2-squareline-wifi/).

## Files

- [`watch_wifi_scan_test.cpp`](watch_wifi_scan_test.cpp) contains the Arduino
  scan and paged-display program.
- [`pin_config.h`](pin_config.h) contains the watch display dimensions, QSPI
  pins, and other local board pin definitions.

## Required core and library

- Espressif Arduino-ESP32 core, which supplies `Arduino.h`, `WiFi.h`, and
  `HWCDC.h`.
- The Waveshare-compatible `Arduino_GFX_Library` used by the official watch
  examples, including support for the CO5300 AMOLED controller.

`pin_config.h` is a project header, not a separately installed Arduino library.

## Arduino IDE steps

1. Install the Espressif ESP32 board package.
2. Install the Waveshare-compatible Arduino_GFX library supplied with the
   official ESP32-S3-Touch-AMOLED-2.06 examples.
3. Create a new Arduino sketch and copy `watch_wifi_scan_test.cpp` into its
   `.ino` file.
4. Copy `pin_config.h` into the same sketch folder.
5. Select `ESP32S3 Dev Module`, OPI PSRAM, the correct flash settings, and the
   watch's serial port.
6. Upload the sketch. Open Serial Monitor at 115200 baud to view the complete
   scan results.

## Measured evidence

Version 1 was physically run on the required watch. The preserved photos show
the scanning screen and nearby network results on the AMOLED display:

- [Version 1 scanning photo](../../../photos/images/v1-wifi-scanning-screen.png)
- [Version 1 results photo](../../../photos/images/v1-wifi-scan-results.png)
- [Photo notes and evidence limits](../../../photos/README.md)

This test supports basic Wi-Fi scanning and display feasibility only. It does
not prove rover communication, map or survey-summary transfer, anomaly display,
five-second delivery, or the clean build of the final integrated system.

## Wi-Fi limitations

- The ESP32-S3 watch scans 2.4 GHz Wi-Fi; it cannot detect a 5 GHz-only network.
- A hidden access point does not broadcast its SSID. Passive scanning cannot
  recover that missing name, so the program displays `<hidden>`.
