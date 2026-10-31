# Watch Version 2 Dependencies and Build Settings

This inventory was taken from the local machine and the exact working source
project on 2026-10-31. Third-party library trees are not vendored here. The
project keeps only its generated UI sources and the board/LVGL configuration
files needed to reproduce the sketch.

## Development tools observed

| Tool | Observed version | Observed location |
| --- | --- | --- |
| Arduino IDE | 2.3.8 | `C:\Users\Admin\AppData\Local\Programs\Arduino IDE` |
| Standalone Arduino CLI | 1.5.1 | `C:\Program Files\Arduino CLI\arduino-cli.exe` |
| Espressif ESP32 Arduino core | 3.3.6 | `C:\Users\Admin\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.6` |

The physical-watch build cache from 2026-10-30 records the same ESP32 core,
board options, and source project. Its application binary is 1,223,712 bytes.
The selected default application partition is 1,310,720 bytes, so this exact
configuration has little remaining program space.

## External libraries

| Include or feature | Source | Install separately? | Exact observed version and location |
| --- | --- | --- | --- |
| `Arduino.h` | Espressif ESP32 Arduino core | No; installed with the board package | Core 3.3.6, path above |
| `WiFi.h` | Espressif ESP32 Arduino core | No | `...\esp32\3.3.6\libraries\WiFi` |
| `Wire.h` | Espressif ESP32 Arduino core | No | `...\esp32\3.3.6\libraries\Wire` |
| `HWCDC.h` | Espressif ESP32 Arduino core | No | Core 3.3.6 |
| `lvgl.h` | LVGL | Yes | LVGL 9.3.0 at `C:\Users\Admin\Documents\Arduino\libraries\lvgl` |
| `Arduino_GFX_Library.h` and CO5300 driver | GFX Library for Arduino | Yes | Waveshare-supplied Arduino_GFX 1.6.0 at `C:\Users\Admin\Documents\Arduino\libraries\Arduino_GFX` |
| `Arduino_DriveBus_Library.h` and FT3168 touch driver | Driver Bus Library Based on Arduino | Yes | Arduino_DriveBus 1.0.1 at `C:\Users\Admin\Documents\Arduino\libraries\Arduino_DriveBus` |

The machine also contains GFX Library for Arduino 1.6.9 in
`C:\Users\Admin\Documents\Arduino\libraries\GFX_Library_for_Arduino`.
The successful cached build resolved `Arduino_GFX_Library.h` to the Waveshare
1.6.0 folder named `Arduino_GFX`. Avoid leaving two libraries that provide the
same header enabled during a clean build; use the Waveshare 1.6.0 copy for this
prototype.

## Project-local generated/configuration files

- `ui*.c` and `ui*.h` are the SquareLine Studio 1.6.3 export adapted from its
  original LVGL 8.3.11 output to LVGL 9.3.0. They are application sources, not
  an Arduino Library Manager dependency.
- `pin_config.h` was copied from the locally installed Waveshare
  `board_config` folder. It defines the CO5300 display, touch, and SD pins for
  the exact 2.06-inch board.
- `lv_conf.h` was copied from the working local setup. It selects 16-bit color,
  LVGL's no-OS mode, and the font/configuration options used by this UI.

The board configuration comes from Waveshare's Apache-2.0-licensed
ESP32-S3-Touch-AMOLED-2.06 example repository. `lv_conf.h` is an LVGL
configuration file from the installed MIT-licensed LVGL package. The `ui*`
files are the user's project-local generated/adapted SquareLine output.

## Exact board options recorded by the successful build cache

| Arduino Tools option | Value |
| --- | --- |
| Board | `ESP32S3 Dev Module` |
| Upload Speed | `921600` |
| USB Mode | `Hardware CDC and JTAG` |
| USB CDC On Boot | `Enabled` |
| USB Firmware MSC On Boot | `Disabled` |
| USB DFU On Boot | `Disabled` |
| Upload Mode | `UART0 / Hardware CDC` |
| CPU Frequency | `240MHz (WiFi)` |
| Flash Mode | `QIO 80MHz` |
| Flash Size | `32MB (256Mb)` |
| Partition Scheme | `Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)` |
| Core Debug Level | `None` |
| PSRAM | `OPI PSRAM` |
| Arduino Runs On | `Core 1` |
| Events Run On | `Core 1` |
| Erase All Flash Before Sketch Upload | `Disabled` |
| JTAG Adapter | `Disabled` |
| Zigbee Mode | `Disabled` |
| Serial Monitor | `115200 baud` |

## Arduino IDE build

Open `version-2-squareline-wifi.ino`, select the settings above, and choose
**Sketch > Verify/Compile**. The `.ino` contains the exact tested application
logic, and Arduino automatically compiles every adjacent root-level `.c` UI
file. `reference/watch_squareline_wifi_v2.cpp` is a byte-identical reference
copy outside Arduino's compiled sketch roots; it must not be moved beside the
`.ino`, because doing so would duplicate `setup()` and `loop()`. Then select
the watch COM port and click **Upload**.

If upload stalls at `Connecting...`, hold the watch BOOT button while
reconnecting USB, begin upload, and release BOOT when writing starts.

## Command-line build

The equivalent local command is:

```powershell
& 'C:\Program Files\Arduino CLI\arduino-cli.exe' compile `
  --config-file 'C:\Users\Admin\.arduinoIDE\arduino-cli.yaml' `
  --fqbn 'esp32:esp32:esp32s3:UploadSpeed=921600,USBMode=hwcdc,CDCOnBoot=cdc,MSCOnBoot=default,DFUOnBoot=default,UploadMode=default,CPUFreq=240,FlashMode=qio,FlashSize=32M,PartitionScheme=default,DebugLevel=none,PSRAM=opi,LoopCore=1,EventsCore=1,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default' `
  --libraries 'C:\Users\Admin\Documents\Arduino\libraries' `
  --build-path 'C:\Users\Admin\AppData\Local\Temp\codex-v2-watch-build-20261031' `
  'C:\Users\Admin\Documents\SCHOOL\remote-environmental-survey\src\watch\version-2-squareline-wifi'
```

This command depends on the locally installed toolchain and paths. A future
repository-level clean-build command still needs to be created to satisfy
REQ-RS-08.

## Verification record

- The Arduino IDE cache for the original working project records a successful
  build on 2026-10-30 with the exact FQBN options above. It resolved
  `Arduino_GFX` 1.6.0, `Arduino_DriveBus` 1.0.1, LVGL 9.3.0, and ESP32 core
  3.3.6. The resulting application binary was 1,223,712 bytes with SHA-256
  `1D6F69E6F3BA8E23E0D0A9DD52DBD18B2A59726B7E5DE76FCEE8F1683EF990F6`.
- On 2026-10-31 the command above was run against this packaged directory. In
  the managed sandbox, the compiler stopped before source compilation with
  `{Illegal System DLL Relocation}` while launching
  `xtensa-esp32s3-elf-g++.exe`.
- The same compile was then run outside the managed sandbox. Arduino's shared
  cache contained truncated object archives, so it was rerun with the fresh
  isolated `--build-path` shown in the documented command. The build completed
  successfully with no application-source changes.
- Program-storage result: 1,223,571 bytes used (93%) of the 1,310,720-byte
  application partition.
- Internal dynamic-memory result: 112,992 bytes used (34%) of 327,680 bytes,
  leaving 214,688 bytes for local variables.
- Generated application binary: `version-2-squareline-wifi.ino.bin`, exactly
  1,223,712 bytes, SHA-256
  `0BB18CE51CBFF62930D69B09DE29FA36F207B979F052ED6290AAF53D2D5C6CB5`.
- The original physically tested `.ino`, packaged compiled `.ino`, and
  noncompiled reference `.cpp` are each 11,990 bytes and share SHA-256
  `47B9BCD6A4BD36D65DA36E0E4CAF5326464BBB2D46E384CC57D8B09672A40B65`.
- An earlier package layout placed the tested implementation in a compiled
  root-level `.cpp` with a separate entry `.ino`. The final layout instead puts
  the byte-identical tested source directly in
  `version-2-squareline-wifi.ino` and keeps the named C++ copy only under
  `reference/`, avoiding duplicate `setup()` and `loop()` compilation.

