# ESP32 rover Wi-Fi drive and mapping prototype

The user identified [`sketch_oct31b/sketch_oct31b.ino`](sketch_oct31b/sketch_oct31b.ino) as the sketch used in a phone-controlled rover test. On 2026-11-19 the user reported that the two-motor rover travelled **more than 2 m** under phone Wi-Fi control. This is a user-reported physical subsystem result. No distance record, test photograph, or operator-outside-room observation was supplied with this source, so the exact `REQ-RS-01` acceptance test remains open.

The sketch also includes wheel-encoder odometry, three VL53L0X distance sensors, and a live occupancy-grid page. The presence of that code does not establish mapping accuracy. The eight-wall-point comparison across five runs required by `REQ-RS-05` has not been measured.

The repository sketch compiled successfully with ESP32 Arduino core 3.3.6 for **ESP32 Dev Module**: 990,415 bytes of program storage (75%) and 49,432 bytes of global memory (15%). This build result verifies compilation of this repository copy; it is separate from the user's reported physical drive and does not verify the exact `REQ-RS-01` conditions or mapping accuracy.

## Build and run notes

- Install the ESP32 Arduino core and the **Pololu VL53L0X** Arduino library. `WiFi`, `WebServer`, and `Wire` come from the ESP32/Arduino platform.
- Set `AP_PASS` to a private password of at least eight characters before flashing. The repository copy replaces only the fixed demonstration AP password with `REPLACE_WITH_PRIVATE_PASSWORD`; the user's original tested local source was left unchanged.
- Check every motor, encoder, and sensor pin against the actual rover wiring before uploading. The sketch requests an ESP32 access point named `ESP32-Robot` and prints its IP address; a phone on that network opens the web page for control and map display.
- `COUNTS_PER_WHEEL_REV`, `WHEEL_DIAMETER_MM`, and `WHEEL_BASE_MM` are explicitly marked as placeholders in the sketch. Measure and calibrate them before using odometry as distance or mapping evidence.

## Evidence boundary

The reported >2 m phone drive supports feasibility of wireless motor control. To claim `REQ-RS-01` is met, preserve a measured 2 m path result and show that the operator stayed outside the room. To claim `REQ-RS-05` is met, compare the mapped wall positions with eight tape-measured points in each of five runs and retain all errors. This prototype does not demonstrate environmental readings (`REQ-RS-03`) or the gas-anomaly rule (`REQ-RS-04`).
