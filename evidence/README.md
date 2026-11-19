# Project evidence index

This index separates working subsystem demonstrations from the complete acceptance tests in [`research/requirements.md`](../research/requirements.md). Code, screenshots, and simulations are useful feasibility evidence; each requirement remains open until its stated conditions are measured and recorded.

| Requirement | Current evidence | Acceptance work still open |
| --- | --- | --- |
| `REQ-RS-01` — remote drive | [ESP32 rover control sketch](../src/rover/version-2-mapping-wifi-control/README.md). The user reports a phone-controlled physical drive of more than 2 m. | Preserve a measured path record and document that the operator stayed outside the room. |
| `REQ-RS-02` — phone video | [2026-11-19 camera test](req-rs-02-camera-stream/2026-11-19/README.md): laptop/desktop-to-phone live feed reported by the user, with source code and a phone-browser screenshot. | Run on the intended rover camera/computer and measure phone display latency against the 1 s limit. |
| `REQ-RS-03`–`REQ-RS-05` — mapped readings, anomaly threshold, wall accuracy | [Reproducible Python simulations](../simulations/requirements/README.md) and their generated outputs. Ten software tests passed. | Physical sensor records and map, real anomaly response, and five wall-mapping runs against eight tape-measured points each. The simulations alone do not pass these physical requirements. |
| `REQ-RS-06`, `REQ-RS-09` — watch output | [Standalone phone photo-upload test](../src/watch/version-3-phone-photo-upload-test/README.md), reported working on the required watch. Earlier [Version 1 Wi-Fi scan](../src/watch/version-1-basic-wifi/README.md) and [Version 2 SquareLine UI](../src/watch/version-2-squareline-wifi/README.md) have [runtime photos](../photos/README.md). | Transfer the completed rover survey within 5 s and display its map, summary, and anomaly indication on the exact watch. A photo upload or Wi-Fi scan alone does not pass those requirements. |

The camera screenshot contains a person's face. Review that file before sharing the repository or evidence package beyond the project team.
