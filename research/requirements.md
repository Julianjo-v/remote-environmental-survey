# Project Requirements

This file preserves the requirement IDs and statements from the project proposal. Acceptance tests listed here are planned methods; no test results are claimed.

| ID | Class | Requirement statement |
| --- | --- | --- |
| REQ-RS-01 | Functional | The unit shall complete a 2 m remotely commanded path with the operator outside the room. |
| REQ-RS-02 | Interface | The system shall display live camera video on the operator's phone with latency at most 1 s. |
| REQ-RS-03 | Functional | The map shall show at least 3 location-tagged records, each containing temperature, humidity and relative gas readings. |
| REQ-RS-04 | Functional | The map shall flag a location as a potential gas anomaly when its test reading is at least 125% of baseline. |
| REQ-RS-05 | Performance | Mapped wall positions shall be within +/-10 cm of tape-measured positions in at least 4 of 5 runs. |
| REQ-RS-06 | Interface | The watch shall receive the survey summary within 5 s of survey completion. |
| REQ-RS-07 | Constraint | Total hardware cost shall not exceed CAD $700. |
| REQ-RS-08 | Buildability | The unit's firmware shall build from a clean copy using one documented command. |

## Notes

- The relative gas measurement is an anomaly indicator, not a safety limit.
- The proposal's proof-of-concept focus is REQ-RS-05.
- Results must not be entered until the corresponding tests have been completed and documented.

