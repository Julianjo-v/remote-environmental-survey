# Watch Source Code

This directory contains two preserved watch prototypes for the required
Waveshare ESP32-S3-Touch-AMOLED-2.06. Version 2 supplements Version 1; it does
not overwrite or silently redefine the earlier evidence.

## Version 1: basic Wi-Fi scanner

- [`version-1-basic-wifi/`](version-1-basic-wifi/) preserves the historical
  basic scanner and its setup notes.
- [`version-1-basic-wifi/watch_wifi_scan_test.cpp`](version-1-basic-wifi/watch_wifi_scan_test.cpp)
  is the basic Arduino_GFX scanner.
- [`version-1-basic-wifi/pin_config.h`](version-1-basic-wifi/pin_config.h)
  contains its board pin and display definitions.
- It scans nearby 2.4 GHz networks and displays SSID, RSSI, and channel results
  in automatically changing pages.
- Version 1 documentation and photos are in [`../../photos/`](../../photos/).

## Version 2: SquareLine/LVGL interface

- [`version-2-squareline-wifi/`](version-2-squareline-wifi/) contains the
  byte-identical tested Arduino sketch, a noncompiled clearly named C++
  reference copy, generated UI source, and project-local board/LVGL
  configuration.
- The interface uses touch input, a Wi-Fi enable switch, asynchronous scanning,
  and a scrollable, clickable list of up to 24 results.
- The user reports that Version 2 was flashed to and run on the physical watch;
  the searching and result screens are supported by the supplied photos.
- A non-broadcast SSID may correctly appear as `<hidden>`. Passive scanning
  cannot recover a name that the access point does not advertise.
- Exact observed dependencies, board options, and build instructions are in
  [`version-2-squareline-wifi/LIBRARIES.md`](version-2-squareline-wifi/LIBRARIES.md).
- A fresh isolated Arduino CLI build using that documented target and dependency
  set succeeded. The tested original `.ino`, packaged `.ino`, and reference
  `.cpp` hashes match. The sketch used 1,223,571 bytes (93%) of the 1,310,720-byte
  program partition and 112,992 bytes (34%) of global memory, leaving 214,688
  bytes. The 1,223,712-byte binary SHA-256 is
  `0BB18CE51CBFF62930D69B09DE29FA36F207B979F052ED6290AAF53D2D5C6CB5`.

Both versions are watch-subsystem evidence only. The final rover connection,
survey-summary transfer, map display, anomaly display, five-second delivery,
and clean one-command build of the integrated system remain unfinished.
