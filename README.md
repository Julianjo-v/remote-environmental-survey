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

- Version 1 watch firmware was physically tested on the required Waveshare
  watch. It scanned nearby 2.4 GHz Wi-Fi networks and displayed network names,
  signal strength, and channel information on the AMOLED screen.
- The Version 1 source is stored in
  [`src/watch/`](src/watch/), and its test photos are documented in
  [`photos/`](photos/).
- The current Stage 1 Concept and Feasibility Report was compiled and verified
  as a four-page PDF:
  [report source and PDF](paper/stage-1-concept-and-feasibility-report/).

The user also reports that a later SquareLine Studio/LVGL graphical Wi-Fi
interface ran successfully on the physical watch. Its implementation was
inspected locally and included a touch switch plus a scrollable, clickable
network list. The later source, runtime artifacts, and a clean repository build
are not currently preserved here, so this is not claimed as a completed
repository build or as proof of the final survey interface.

## Planned components and architecture

The Bosch BME688, Raspberry Pi Zero 2 W, and a 3D-printed tracked-tank platform
remain planned candidates. Their final compatibility, power requirements,
mechanical integration, mapping accuracy, and complete system roles have not
been verified. No final rover architecture has been selected.

Project requirements [REQ-RS-01 through REQ-RS-10](research/requirements.md)
cover remote movement, phone video, tagged environmental readings, relative
anomaly indication, map accuracy, watch delivery, cost, reproducible builds,
the required watch, and the final custom PCB.

## Watch Version 1 test

- [Watch source and evidence limits](src/watch/README.md)
- [Version 1 research photos and test notes](photos/README.md)

The Version 1 test supports basic watch display and Wi-Fi scanning feasibility.
It does not prove rover connection, survey-summary transfer, map display,
five-second delivery, simultaneous video traffic, or a clean one-command build.

## Open work

- Select and justify the rover controller architecture.
- Implement and test rover motion, live phone video, mapping, and sensing.
- Bench-test the BME688 as a relative anomaly sensor.
- Send a completed survey map and tagged readings to the watch and measure delivery time.
- Verify mapping accuracy, video latency, battery/power behavior, and hardware cost.
- Produce a clean, documented one-command firmware build.
- Design and integrate the required final custom PCB.
- Preserve later SquareLine/LVGL source and runtime evidence in the repository.

## Repository structure

- [`paper/`](paper/) — proposal material and the current Stage 1 report.
- [`paper/stage-1-concept-and-feasibility-report/`](paper/stage-1-concept-and-feasibility-report/) — LaTeX source, verified four-page PDF, and report notes.
- [`photos/`](photos/) — Version 1 watch research photos and explanation.
- [`research/`](research/) — sources, research notes, and requirement tracking.
- [`src/`](src/) — project source-code organization.
- [`src/watch/`](src/watch/) — current Version 1 watch firmware.
- [`src/rover/`](src/rover/) — placeholder for future rover code; no rover implementation is present yet.

## Safety and evidence limits

This is an educational prototype, not a certified gas detector or room-safety
instrument. The BME688 is planned for broad relative environmental response;
it must not be described as selective toxic-gas detection or proof that a room
is safe. Requirements remain open or partly supported until their planned tests
are completed and documented. Planned features, user-reported results, and
manufacturer specifications are kept separate from measured project evidence.

