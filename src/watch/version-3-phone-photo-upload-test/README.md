# Watch photo upload test

This is a separate test for the Waveshare ESP32-S3-Touch-AMOLED-2.06. It does
not change `square_line3` and does not use the SD card. The watch starts its own
2.4 GHz Wi-Fi network, and a phone sends one photo to it. The phone browser
converts the chosen image to a small RGB565 picture for the watch screen. The
watch keeps only the latest photo in PSRAM until power is removed.

## Run it

1. Open [`watch_photo_upload_test/watch_photo_upload_test.ino`](watch_photo_upload_test/watch_photo_upload_test.ino) in Arduino IDE. Its pin header is in the same sketch folder. The sample `test-photo.png` remains beside this README.
2. Install **GFX Library for Arduino** (the Waveshare-compatible Arduino_GFX
   library) and use the **ESP32 Arduino core**.
3. Replace the `AP_PASSWORD` placeholder in the sketch with a private password
   of at least eight characters before flashing. Do not commit that value.
4. Select **ESP32S3 Dev Module**, **32 MB flash**, **OPI PSRAM**, and **USB CDC
   On Boot: Enabled**. Select the watch port, compile, and upload.
5. On the phone, join the watch's 2.4 GHz Wi-Fi network **WatchPhotoTest** with
   the password you set. A phone may say this network has no internet; stay on it.
6. Open the address shown on the watch, normally `http://192.168.4.1`.
7. Select a JPEG, PNG, or WebP photo and tap **Show on watch**. A preview appears
   in the phone browser, and the watch displays the transferred picture.

The browser fits images within 320 x 380 pixels, preserves aspect ratio, and
sends 16-bit RGB565 pixels. A second upload replaces the first photo. No files
are written to the watch's SD card or flash. The supplied `test-photo.png` is
2.9 MB on disk; select it through the page and let the browser resize and
convert it. Do not send the original file directly to `/upload`.

## Test boundary

The user reported on **2026-11-13** that the original local version of this
standalone sketch worked on the physical watch: a phone joined the watch's
Wi-Fi, uploaded a photo, and the watch displayed it. The repository copy has
only its fixed demonstration AP password replaced by a placeholder. The
original tested local source was not changed. After setting a private password,
this repository copy still needs its own flash test before its behaviour can be
claimed as independently verified.

This is evidence of a photo-transfer subsystem. It does not prove SD-card
writing, a complete rover survey summary, or delivery within five seconds
(`REQ-RS-06`). It also does not yet satisfy the full watch output requirement
(`REQ-RS-09`), which includes a map, summary, and anomaly indication.

The repository copy compiled successfully with ESP32 Arduino core 3.3.6 for
**ESP32S3 Dev Module**, 32 MB flash, Huge APP partition, OPI PSRAM, and USB CDC
enabled. It used 1,022,327 bytes of program storage (32% of the 3 MB app
partition) and 46,832 bytes of global memory (14%). This verifies compilation
of the password-redacted copy, not a physical flash or repeat upload test.
