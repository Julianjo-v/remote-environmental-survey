# REQ-RS-02: phone live-video test — 2026-11-19

The user reports that live video appeared on a phone on 2026-11-19. The supplied screenshot shows a phone browser displaying a camera image at a local network address. This is evidence of the phone display and the user's reported live-feed test; a still screenshot cannot establish frame motion or delay.

## Files

- [`camera_stream.py`](camera_stream.py) is an unchanged copy of the Python script used for the test. It captures camera index 0 with OpenCV, JPEG-encodes frames, and serves an MJPEG stream through Flask on port 5000.
- [`phone-live-feed.png`](phone-live-feed.png) is the screenshot supplied by the user. It contains a person's face, so consider privacy before sharing this evidence outside the project.
- [`requirements.txt`](requirements.txt) lists the Python packages imported by the script. `socket` is part of Python's standard library.

## Reproduce the prototype

On a computer with a camera and Python installed:

```text
python -m pip install -r requirements.txt
python camera_stream.py
```

Connect the phone to the same Wi-Fi network and open the URL printed by the script. The script requests 1280 x 720 capture and JPEG quality 70; actual camera output and performance were not measured in this record.

## Evidence status

**Partly supported, not yet passed.** The user clarified that this test streamed from a laptop or desktop computer to the phone. The camera model was not recorded, and operation on the planned Raspberry Pi Zero 2 W is not established here. The requirement also calls for phone video latency of at most 1 second. No timed camera-event-to-phone-display trials were supplied, so that limit remains open. Record the hardware, network, dated observations, and repeated latency measurements before marking the requirement supported.

Related software-only evidence for [`REQ-RS-03`–`REQ-RS-05`](../../../simulations/requirements/README.md) and the existing [watch test record](../../../src/watch/README.md) are kept separately. Those subsystem results do not prove the complete rover survey.
