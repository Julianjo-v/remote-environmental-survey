# Project Memory

Last updated: 2026-10-31

## Project

- Title: Remote Environmental Survey and Mapping of an Unseen Room
- Student: Julian Joseph Amaro
- Course: Project I 247-519-VA at Vanier College
- Repository purpose: research, requirements, evidence, and report material for the project

## Durable User Preferences

- Answer the specific question asked and do not expand a narrow review into a full-project evaluation.
- When asked about the watch, use the watch as the reference and evaluate only whether the proposed use stays within its capabilities.
- Clearly distinguish documented capability, engineering inference, planned testing, and verified test results.
- Do not invent measurements or results.
- The project uses the seven shared roles defined in
  `GAS ROBOT AGENTS/AGENTS.md`: GENERAL QUESTION, Planning, Hardware,
  watchesp32, Document Reader/Maker, GITHUB REPO, and Logbook.
- Hardware works as the project's Hardware Inspector.
- Hardware inspects the project's external components, electrical interfaces,
  power, physical integration, availability, cost, and compatibility evidence.
- WatchESP32 is the existing Codex chat titled `watchesp32`, thread
  `01a16903-fecf-7e42-8dd0-1a9fcd1dfffd` on host `local`.
- Hardware and WatchESP32 work together whenever a hardware choice affects the
  required watch. WatchESP32 owns the final watch-capability verdict.
- Document Reader/Maker owns project PDFs, assignment guides, literature,
  report work, document verification, and document-derived requirements.

## Required Watch

- Exact model: Waveshare ESP32-S3-Touch-AMOLED-2.06
- This exact watch must be used as a system output in the final project and
  must display the project map. A phone-only or substitute-display map is not
  sufficient.
- Confirmed core specifications from official sources:
  - ESP32-S3R8 processor, up to 240 MHz
  - 8 MB PSRAM and 32 MB flash
  - 2.06-inch 410 x 502 AMOLED capacitive touchscreen
  - 2.4 GHz 802.11b/g/n Wi-Fi
  - Bluetooth 5 Low Energy
  - QMI8658 six-axis IMU
  - PCF85063 RTC
  - AXP2101 power-management IC
  - microSD card slot
  - Arduino and ESP-IDF development support
- Do not claim Wi-Fi 6 or 802.11ax support.
- Waveshare states that the device is not waterproof.
- The recommended battery is approximately 400 mAh. Vendor guidance is approximately one hour at full brightness, three to four hours with the display off, and about six hours in low-power conditions. These are vendor estimates, not project test results.
- Wi-Fi and BLE share one 2.4 GHz radio and simultaneous heavy use requires testing.

## Current Acceptable Watch Role

The following use is within the watch's expected capabilities:

- Receive a compact survey summary wirelessly.
- Display the project map; a simplified room map or readable map thumbnail is
  acceptable if it preserves the required mapped information.
- Display at least three tagged temperature, humidity, and relative gas records.
- Display a potential-anomaly indicator.
- Provide basic touch navigation and acknowledgement.
- Show connection, timestamp, and battery status.

The following should not be assumed without a new decision and supporting tests:

- Full rover mapping or navigation processing on the watch.
- Motor control as the watch's primary responsibility.
- Continuous high-quality live camera video on the watch.
- Direct environmental sensing without external sensors.
- Certified gas detection or room-safety decisions.
- Reliable heavy Wi-Fi and BLE traffic at the same time.
- Waterproof or all-day full-brightness operation.

## Current Project Scope

- Remotely operated mobile unit surveys one unfamiliar enclosed room.
- Phone is intended for drive commands and live video.
- Rover is intended to perform motion, environmental sensing, mapping, and survey-record generation.
- Watch is required to receive and display the completed survey summary,
  including the project map.
- The final system is required to include a custom PCB; breadboards, loose
  modules, and development boards alone do not satisfy this constraint.
- Relative gas measurement is an anomaly indicator only.
- Autonomous navigation, three-dimensional mapping, multiple-room mapping, cloud storage, and safety certification are outside the current scope.

## Current Planned Components — Not Yet Verified

- Rover platform: the “Fully 3D Printed TANK” tracked-rover design referenced
  by the user. The chassis, tracks, suspension, sprockets, and motor mounts are
  planned as the mechanical base. Its suitability with the final motors,
  encoders, payload, battery, custom PCB, and mapping-accuracy requirement has
  not yet been demonstrated.
- Environmental and relative-gas sensor: Bosch BME688. It is planned for
  temperature, humidity, pressure, and broad relative-gas response. It has not
  been bench-tested for this rover, and it must not be treated as selective or
  certified hazardous-gas detection.
- These are planned candidate components, not evidence that the integrated
  system works.

## Requirement Records

- REQ-RS-01: Complete a 2 m remotely commanded path with the operator outside the room.
- REQ-RS-02: Display live camera video on the phone with latency at most 1 s.
- REQ-RS-03: Map at least three tagged records containing temperature, humidity, and relative gas readings.
- REQ-RS-04: Flag a potential gas anomaly at 125 percent or more of baseline.
- REQ-RS-05: Map wall positions within plus or minus 10 cm in at least four of five runs.
- REQ-RS-06: Deliver the survey summary to the watch within 5 s of survey completion.
- REQ-RS-07: Keep total hardware cost at or below CAD 700.
- REQ-RS-08: Build firmware from a clean copy using one documented command.
- REQ-RS-09: Use the exact Waveshare ESP32-S3-Touch-AMOLED-2.06 watch as an output that displays the project map, survey summary, and anomaly indications.
- REQ-RS-10: Include a custom PCB in the final system.

## Evidence Status

- Official documentation supports the watch's suitability as a compact wireless display and user interface.
- Project-specific basic Version 1 watch firmware is recorded at
  `src/watch/watch_wifi_scan_test.cpp`, and measured Wi-Fi scan/display evidence
  is incorporated into the Stage 1 report.
- Version 2 SquareLine/LVGL source, documentation, and runtime photos are in
  `src/watch/version-2-squareline-wifi/` and `photos/images/v2*`. The exact
  tested source, packaged source, and reference-source hashes match, and a
  physical flash/run is evidenced.
- A fresh isolated build of the documented Version 2 Arduino CLI package passed:
  program storage was 1,223,571 bytes (93 percent), global variables were
  112,992 bytes (34 percent), and 214,688 bytes remained for local variables.
- Hidden nonbroadcast SSIDs correctly appear as `<hidden>`; a passive scan
  cannot recover a network name that the access point omits.
- The final rover link, map, summary, anomaly indication, five-second transfer,
  and full integrated REQ-RS-08 build remain open.
- No requirement should be marked passed until its planned test is performed and documented.

## Verified Repository Milestones

- 2026-10-23 — The GITHUB REPO agent verified that commit
  `f3be78704ab5a337d6fa42d3eab224ae2bfe47b5` was pushed to `origin/main` for
  `Julianjo-v/remote-environmental-survey`. The commit contains only
  `stage1-research-draft/main.tex` and `stage1-research-draft/main.pdf`.
- 2026-10-30 — The GITHUB REPO agent created and pushed
  `watch-tests/wifi-connection/src/.gitkeep` in commit
  `0305f1b7b49722766a7f8afa2f18addd2f2ebcf5` to `origin/main`, with the remote
  state verified.
- 2026-10-30 — The GITHUB REPO agent pushed commit
  `df0a8f006e309a1a90f7e3a4ef65112e3dcacf66` to `origin/main`, with the remote
  state verified, documenting the watch Wi-Fi scan test through a README and
  two images, renaming `ESP32_WIFITEST.c` to `src/main.cpp`, and removing
  `src/.gitkeep`. The build remains unverified because the ESP32-S3 Arduino CLI
  compile stalled and was stopped without producing a result.
- 2026-10-30 — The GITHUB REPO agent pushed commit
  `d4ff6d1651171a2199453466b2cba012c46ff9ed` to `origin/main`, with the remote
  state verified. It moved the watch code to the repository-root
  `src/main.cpp` and `src/pin_config.h`, moved the documentation and images to
  `docs/watch-wifi/`, and removed all `watch-tests/` paths while preserving the
  moved files' hashes and content. The root `src/` and `docs/watch-wifi/`
  locations are now current; earlier `watch-tests/` references above are
  retained only as historical milestone records.
- 2026-10-30 — The GITHUB REPO agent pushed commit
  `36ada56def9075cb8dac3584b41aae9a9d0fe2b5` to `origin/main` and remotely
  verified it. The current basic Version 1 watch Wi-Fi scan source is
  `src/watch_wifi_scan_test.cpp`, its documentation is
  `docs/watch-wifi/README.md`, and its image filenames use `v1-` prefixes. The
  README explicitly identifies this as the historical/basic scanner and
  distinguishes it from later SquareLine/LVGL UI work, which is not included
  in the repository.
- 2026-10-30 — The GITHUB REPO agent pushed commit
  `0065e4805becc884518e0d8577004842c1e33e09` to `origin/main` and remotely
  verified it; the commit contains only `stage1-research-draft/main.tex`, and
  the built-in LaTeX compile succeeded. The report now incorporates measured
  Version 1 watch Wi-Fi scan/display evidence. SquareLine/LVGL work, rover
  connection, summary/map transfer, five-second delivery, pagination retest,
  and a clean one-command build remain open.
- 2026-10-31 — The GITHUB REPO agent pushed commit
  `a2b1affa9c29453ae3d3976854c1d109041e7245` to `origin/main` and remotely
  verified it. The current structure places Version 1 watch firmware at
  `src/watch/watch_wifi_scan_test.cpp` and `src/watch/pin_config.h`, research
  photos in `photos/`, and the Stage 1 Concept and Feasibility Report at
  `paper/stage-1-concept-and-feasibility-report/` with `main.tex`, `main.pdf`,
  and `README.md`; the old `report/`, `docs/`, and `stage1-research-draft/`
  paths were removed. The report PDF was regenerated and verified as four
  pages after two `pdflatex` passes. SquareLine/LVGL graphical Wi-Fi interface
  success is user-reported and locally inspected, but its later source/runtime
  artifacts and clean build are not in this repository, so final map, summary,
  anomaly, and transfer evidence remains open. Preserved local edits adding
  REQ-RS-09 and REQ-RS-10 to `research/requirements.md` remain uncommitted and
  were not part of this commit.
- 2026-10-31 — The GITHUB REPO agent pushed commit
  `dda1f3ee90a2bd611378729c00ef6394f0d44376` to `origin/main` and remotely
  verified it. The commit contains only the root `README.md`, refreshing the
  current Stage 1 overview, agent roles, evidence, open work, safety boundaries,
  and repository structure; all relative links were verified. No tag or release
  was created. Preserved uncommitted research files were excluded from the
  commit.
- 2026-10-31 — The GITHUB REPO agent pushed commit
  `1f47779613218d3c0e6bee196dc11b07b659379a` to `origin/main` and remotely
  verified it. The commit adds the Version 2 SquareLine/LVGL source,
  documentation, and runtime photos and regenerates the Stage 1 report as a
  verified four-page PDF. The remote tag `v1.0.0-stage1` was observed pointing
  to earlier commit `f6d8f99445f569d698d6b160f4b7fc17349e9e61`, so that tag excludes Version 2;
  no tag change was authorized or performed.

## Stage 1 Concept and Feasibility Report Guide

- Controlling guide: `C:\Users\Admin\.codex\codex-remote-attachments\01a16923-3535-7040-8fc2-43ef5c422655\2250B23B-48EC-4C37-BD04-EE13AF8CAD26\1-Part_B_Writing_the_Stage_1_Concept_and_Feasibility_Report.pdf`.
- Submission is an individual, printed, three-to-four-page report with figures legible in black and white.
- Use the required six-section skeleton: Change Log, Concept Alternatives, Feasibility Case, Baselined Requirements, Risk Register, and Gate 1 Readiness, preceded by the proposal title block.
- Preserve every proposal identifier. Continue numbering for additions; never restart, renumber, or reuse a retired identifier.
- Record every addition, retirement, scope change, or other departure from the frozen proposal in the change log and tie it to the dated logbook entry that explains it.
- Compare at least two genuinely different concepts. Fix the evaluation criteria and weights before scoring, explain why each rejected concept lost, and include a functional block diagram of the selected concept.
- Classify evidence as measured, calculated, or assumed and identify its source. Assign claims and each requirement one honest status: supported, partly supported, unsupported, or open.
- Baseline every requirement with its identifier, reason, evidence status, and any change-log reference; do not silently relax stakeholder constraints.
- Rank risks using probability times consequence (`P = p x c`), show the matrix, and write the highest risks in the required six-field form.
- State an explicit Gate 1 outcome: proceed, proceed with conditions, or re-scope, including remaining gaps and the next evidence needed.
- Stage 1 implementation is low fidelity and may use breadboards, modules, perfboard, or protoboard. PCB design and manufacture belong to Project II; the final custom-PCB requirement remains binding but Stage 1 PCB fabrication is not required.
- Prepare the costed plan and bill of materials separately in Week 4 rather than placing them in the three-to-four-page Stage 1 report.

## Primary Watch Sources

- Waveshare product page: https://www.waveshare.com/esp32-s3-touch-amoled-2.06.htm
- Waveshare wiki: https://www.waveshare.com/wiki/ESP32-S3-Touch-AMOLED-2.06
- Espressif ESP32-S3 datasheet: https://documentation.espressif.com/esp32-s3_datasheet_en.pdf
- Espressif RF coexistence guide: https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-guides/coexist.html

## Open Questions

- Which wireless path will be used for the final compact summary: direct Wi-Fi from rover to watch or a phone-mediated BLE transfer?
- What display brightness and duty cycle are required for the demonstration duration?
- What is the measured end-to-end watch delivery time under simultaneous phone video traffic?
