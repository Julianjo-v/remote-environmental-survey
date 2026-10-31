# Watch Source Code

This directory contains the current repository source for the required
Waveshare ESP32-S3-Touch-AMOLED-2.06 watch.

## Version 1 evidence

- `watch_wifi_scan_test.cpp` is the basic Version 1 Wi-Fi scanner.
- `pin_config.h` contains the board pin and display definitions used by that scanner.
- The Version 1 test scanned nearby 2.4 GHz networks and displayed network names, signal strength, and channel information on the AMOLED screen.
- The related photos and explanation are stored in `../../photos/`.

This is measured subsystem evidence only. It does not prove connection to the
rover, survey-summary transfer, map display, five-second delivery, or a clean
one-command build from a fresh copy.

## Later graphical interface

The user reports that a later SquareLine Studio/LVGL interface ran successfully
on the physical watch. That interface used touch input, a switch to start the
Wi-Fi scan, and a scrollable, clickable list of results. Its source and runtime
artifacts are not currently included in this repository, so this directory does
not present that later implementation as repository-verified evidence.
