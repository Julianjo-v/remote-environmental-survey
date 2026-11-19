import socket
import cv2
from flask import Flask, Response

CAMERA_INDEX = 0   # try 1 if you have multiple cameras
PORT = 5000
WIDTH, HEIGHT = 1280, 720
JPEG_QUALITY = 70  # lower = faster/less bandwidth

app = Flask(__name__)
cap = cv2.VideoCapture(CAMERA_INDEX)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, WIDTH)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, HEIGHT)


def frames():
    while True:
        ok, frame = cap.read()
        if not ok:
            break
        ok, buf = cv2.imencode(".jpg", frame, [cv2.IMWRITE_JPEG_QUALITY, JPEG_QUALITY])
        if not ok:
            continue
        yield (b"--frame\r\nContent-Type: image/jpeg\r\n\r\n" + buf.tobytes() + b"\r\n")


@app.route("/")
def index():
    return (
        "<body style='margin:0;background:#000;display:flex;height:100vh'>"
        "<img src='/video' style='width:100%;object-fit:contain'></body>"
    )


@app.route("/video")
def video():
    return Response(frames(), mimetype="multipart/x-mixed-replace; boundary=frame")


def local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(("8.8.8.8", 80))
        return s.getsockname()[0]
    except OSError:
        return "127.0.0.1"
    finally:
        s.close()


if __name__ == "__main__":
    print(f"\nOpen this on your phone (same Wi-Fi): http://{local_ip()}:{PORT}\n")
    app.run(host="0.0.0.0", port=PORT, threaded=True)