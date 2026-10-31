# Watch Version 2: SquareLine/LVGL Wi-Fi Interface

This folder packages the second watch prototype for the exact Waveshare
ESP32-S3-Touch-AMOLED-2.06. Version 2 keeps the working SquareLine/LVGL logic
from `ESP32_WATCH_UI_TEST` and does not replace or modify the basic Version 1
scanner in the parent directory.

## Evidence status

The user reports that this interface was flashed to and run on the physical
watch. The supplied photos show the interface searching and then displaying
five Wi-Fi scan results:

- [Version 2 searching on the physical watch](../../../photos/images/v2-squareline-wifi-searching.png)
- [Version 2 results on the physical watch](../../../photos/images/v2-squareline-wifi-scan-results.png)

This is physical subsystem evidence for the watch display, touch-controlled
scan, and result-list interface. It does not yet prove the final rover link,
survey-summary transfer, map display, anomaly display, or five-second delivery
requirement.

Packaging was verified again on 2026-10-31. The compiled
`version-2-squareline-wifi.ino` and the noncompiled
`reference/watch_squareline_wifi_v2.cpp` are byte-identical to the physically
tested `ESP32_WATCH_UI_TEST.ino` source. A fresh Arduino CLI build of the
packaged sketch succeeded with the recorded settings and dependencies: the
application uses 1,223,571 bytes of the 1,310,720-byte application partition,
and global variables use 112,992 of 327,680 bytes of internal dynamic memory.

## Behavior

1. The watch starts with Wi-Fi disabled.
2. Touching the switch enables station mode and starts an asynchronous 2.4 GHz
   scan.
3. The status changes to `SEARCHING...` and then `FOUND n`.
4. Up to 24 results are copied into a scrollable LVGL list.
5. Each result shows its SSID, RSSI in dBm, and Wi-Fi channel.
6. Touching a result changes the status label to `SELECTED: <SSID>` and prints
   the selection over USB serial.
7. Turning the switch off clears the list and powers down the Wi-Fi radio.

The scan does not connect to a network and does not request or store a Wi-Fi
password.

## Hidden-network limitation

The scan asks the ESP32-S3 to include hidden networks. A hidden access point
does not broadcast its SSID in the normal beacon information used by a passive
scan, so the returned name can legitimately be empty. Version 2 displays such
a result as `<hidden>`. Passive scanning cannot recover the missing name; the
SSID must be supplied by the user or learned during an authorized connection
workflow.

## Files

- `version-2-squareline-wifi.ino` is the compiled Arduino sketch and is
  byte-for-byte identical to the physically tested working source.
- `reference/watch_squareline_wifi_v2.cpp` is the user-requested clearly named
  C++ reference copy of the same source. The `reference/` directory is not
  compiled by Arduino, so this named copy cannot create duplicate `setup()` and
  `loop()` definitions.
- `ui.c`, `ui.h`, `ui_Screen1.c`, `ui_Screen1.h`, `ui_helpers.c`,
  `ui_helpers.h`, `ui_events.h`, and `ui_comp_hook.c` are the project-local
  SquareLine-generated/adapted interface sources required by the build.
- `pin_config.h` is the local Waveshare board pin configuration.
- `lv_conf.h` is the local LVGL 9 configuration used by the working project.
- `LIBRARIES.md` records every external dependency, observed version, board
  option, and build command.

## Build and upload

1. Install the dependencies and versions listed in [LIBRARIES.md](LIBRARIES.md).
2. Open `version-2-squareline-wifi.ino` in Arduino IDE. Keep every root-level
   UI/configuration file in this directory; Arduino compiles the adjacent `.c`
   files. The `.cpp` under `reference/` is documentation only and is not part
   of the Arduino build.
3. Apply the recorded `ESP32S3 Dev Module` settings from `LIBRARIES.md`.
4. Verify the sketch, select the watch's COM port, and upload.
5. Open Serial Monitor at 115200 baud for touch coordinates, scan results, and
   selected-network messages.

## Still unfinished for the final project

- Connect the watch to the rover or an approved phone-mediated transport.
- Receive the completed survey summary within five seconds of survey
  completion.
- Display the required room map on the watch.
- Display at least three tagged temperature, humidity, and relative-gas
  records.
- Display the potential-anomaly indication without presenting it as certified
  gas identification or a room-safety decision.
- Add final connection, timestamp, and battery status behavior.
- Establish and document the clean one-command firmware build required by
  REQ-RS-08.

