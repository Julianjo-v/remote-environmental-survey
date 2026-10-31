# Remote Environmental Survey and Mapping of an Unseen Room

This Stage 1 engineering project investigates a remotely operated rover that can
survey one unfamiliar enclosed room without requiring the operator to enter it.
The rover is intended to move through the room, estimate a 2D map, and attach
temperature, humidity, and relative-gas readings to mapped locations.

## System roles

- **Rover:** performs motion, environmental sensing, mapping, and survey-record generation.
- **Phone:** provides remote driving controls and the live camera view.
- **Required watch:** the exact Waveshare ESP32-S3-Touch-AMOLED-2.06 must display the project map and completed survey summary.

The final integrated system must include a custom PCB. Development boards,
breadboards, or modules may support Stage 1 testing, but they do not replace
the final custom-PCB requirement.

## Current verified progress

- Version 2 watch firmware was flashed and run on the required Waveshare watch.
  The SquareLine/LVGL interface responded to its touch switch, showed a
  `SEARCHING` state, reported `FOUND 5`, and presented a scrollable results list
  containing SSID, RSSI, and channel information. The two physical-watch photos
  are measured subsystem evidence.
- The Version 2 source package is preserved in
  [`src/watch/version-2-squareline-wifi/`](src/watch/version-2-squareline-wifi/),
  and the V1/V2 test photos are stored under [`photos/images/`](photos/images/).
- A fresh isolated Arduino CLI build of the packaged V2 sketch succeeded using
  the documented board target and dependencies. The tested original sketch,
  packaged sketch, and reference C++ copy have matching hashes. The build used
  1,223,571 bytes (93%) of the 1,310,720-byte program partition and 112,992
  bytes (34%) of global memory, leaving 214,688 bytes. Its 1,223,712-byte binary
  has SHA-256 `0BB18CE51CBFF62930D69B09DE29FA36F207B979F052ED6290AAF53D2D5C6CB5`.
- Version 1 remains preserved as the earlier basic scan/display test under
  [`src/watch/version-1-basic-wifi/`](src/watch/version-1-basic-wifi/).
- The current Stage 1 Concept and Feasibility Report was compiled and verified
  as a four-page PDF:
  [report source and PDF](paper/stage-1-concept-and-feasibility-report/).

The V2 package, runtime photos, and subsystem build evidence are preserved.
This does not prove the final survey interface or the clean build of the future
integrated rover/watch firmware required by REQ-RS-08.

## Planned components and architecture

The Bosch BME688, Raspberry Pi Zero 2 W, and a 3D-printed tracked-tank platform
remain planned candidates. Their final compatibility, power requirements,
mechanical integration, mapping accuracy, and complete system roles have not
been verified. No final rover architecture has been selected.

Project requirements [REQ-RS-01 through REQ-RS-10](research/requirements.md)
cover remote movement, phone video, tagged environmental readings, relative
anomaly indication, map accuracy, watch delivery, cost, reproducible builds,
the required watch, and the final custom PCB.

## Watch Version 1 and Version 2 tests

- [Watch source and evidence limits](src/watch/README.md)
- [Version 1 basic Wi-Fi source package](src/watch/version-1-basic-wifi/)
- [Version 2 SquareLine/LVGL source package](src/watch/version-2-squareline-wifi/)
- [Research photos and test notes](photos/README.md)
- [V2 searching-state photo](photos/images/v2-squareline-wifi-searching.png)
- [V2 scan-results photo](photos/images/v2-squareline-wifi-scan-results.png)

The V1 and V2 tests support watch display, touch-interface, and 2.4 GHz Wi-Fi
scanning feasibility. A hidden access point does not broadcast its SSID, so a
passive scan cannot recover its name; the interface should display `<hidden>`.
These tests do not prove rover connection, survey-summary transfer, map display,
anomaly indication, five-second delivery, simultaneous video traffic, or a
clean one-command build of the full integrated system.

## Open work

- Select and justify the rover controller architecture.
- Implement and test rover motion, live phone video, mapping, and sensing.
- Bench-test the BME688 as a relative anomaly sensor.
- Send a completed survey map and tagged readings to the watch and measure delivery time.
- Verify mapping accuracy, video latency, battery/power behavior, and hardware cost.
- Produce a clean, documented one-command build for the integrated rover/watch firmware.
- Design and integrate the required final custom PCB.
- Monitor and reduce watch program-partition use as the final survey UI is added.

## Repository structure

- [`paper/`](paper/) — proposal material and the current Stage 1 report.
- [`paper/stage-1-concept-and-feasibility-report/`](paper/stage-1-concept-and-feasibility-report/) — LaTeX source, verified four-page PDF, and report notes.
- [`photos/`](photos/) — Version 1 and Version 2 watch research photos and explanation.
- [`research/`](research/) — sources, research notes, and requirement tracking.
- [`src/`](src/) — project source-code organization.
- [`src/watch/`](src/watch/) — index for the preserved watch prototypes.
- [`src/watch/version-1-basic-wifi/`](src/watch/version-1-basic-wifi/) — preserved historical basic Wi-Fi scan/display source.
- [`src/watch/version-2-squareline-wifi/`](src/watch/version-2-squareline-wifi/) — preserved SquareLine/LVGL Wi-Fi scan interface source package.
- [`src/rover/`](src/rover/) — placeholder for future rover code; no rover implementation is present yet.

## Safety and evidence limits

This is an educational prototype, not a certified gas detector or room-safety
instrument. The BME688 is planned for broad relative environmental response;
it must not be described as selective toxic-gas detection or proof that a room
is safe. Requirements remain open or partly supported until their planned tests
are completed and documented. Planned features, user-reported results, and
manufacturer specifications are kept separate from measured project evidence.

