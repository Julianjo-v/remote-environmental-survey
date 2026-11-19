#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <VL53L0X.h>

// ============================================================
// ESP32 Wi-Fi Remote Control + Live Occupancy Grid Mapping
// Join Wi-Fi "ESP32-Robot", open http://192.168.4.1
// Drive with the buttons, watch the map build on the phone.
// ============================================================

// -------------------- WI-FI --------------------
const char* AP_SSID = "ESP32-Robot";
const char* AP_PASS = "REPLACE_WITH_PRIVATE_PASSWORD";

// -------------------- CALIBRATION (CHANGE THESE!) --------------------
// Encoder counts per full WHEEL revolution (rising edges of channel C1 only)
// = motor encoder pulses per rev x gear ratio.  PLACEHOLDER - measure yours.
const float COUNTS_PER_WHEEL_REV = 1050.0;
const float WHEEL_DIAMETER_MM    = 43.0;   // PLACEHOLDER - measure your wheel
const float WHEEL_BASE_MM        = 90.0;   // PLACEHOLDER - center-to-center of wheels
// Set to -1 if that wheel's count goes DOWN when driving forward
const int LEFT_SIGN  = 1;
const int RIGHT_SIGN = 1;

// -------------------- MOTOR PINS (same as working code) --------------------
const int AIN1 = 26;  // Right motor
const int AIN2 = 27;
const int BIN1 = 32;  // Left motor
const int BIN2 = 33;
const int PWM_FREQ = 20000;
const int PWM_BITS = 8;
const unsigned long COMMAND_TIMEOUT = 400;

// -------------------- ENCODER PINS --------------------
const int ENC_L_A = 18, ENC_L_B = 19;   // left  C1, C2
const int ENC_R_A = 35, ENC_R_B = 34;   // right C1, C2

// -------------------- ToF SENSORS (same as working code) --------------------
#define XSHUT_LEFT   15
#define XSHUT_RIGHT  2
#define XSHUT_CENTER 4
#define ADDR_LEFT    0x30
#define ADDR_RIGHT   0x31
#define ADDR_CENTER  0x32
VL53L0X sensorLeft, sensorRight, sensorCenter;
bool okLeft = false, okRight = false, okCenter = false;
int offsetLeft = 0, offsetRight = 0, offsetCenter = -20;

// Sensor directions relative to robot heading (radians, left = positive)
const float ANGLE_LEFT   =  0.7854;   // +45 deg
const float ANGLE_CENTER =  0.0;
const float ANGLE_RIGHT  = -0.7854;   // -45 deg

// -------------------- MAP SETTINGS --------------------
const int   GRID_N       = 40;     // 40 x 40 cells
const int   CELL_MM      = 100;    // 10 cm per cell -> 4 m x 4 m map
const int   MAX_RANGE_MM = 1000;   // ignore readings beyond this (VL53L0X is unreliable far out)
int8_t grid[GRID_N * GRID_N];      // log-odds: <0 free, >0 occupied, 0 unknown

// -------------------- STATE --------------------
WebServer server(80);
unsigned long lastCmd = 0;
bool moving = false;

volatile long leftTicks = 0, rightTicks = 0;
long lastL = 0, lastR = 0;
float poseX = 0, poseY = 0, poseTheta = 0;   // mm, mm, radians (start = grid center, facing +X)
const float MM_PER_COUNT = (PI * WHEEL_DIAMETER_MM) / COUNTS_PER_WHEEL_REV;

unsigned long lastSense = 0;

// ============================================================
// ENCODER INTERRUPTS (x1 decoding: count on rising edge of A)
// ============================================================
void IRAM_ATTR isrLeft()  { if (digitalRead(ENC_L_B)) leftTicks++;  else leftTicks--;  }
void IRAM_ATTR isrRight() { if (digitalRead(ENC_R_B)) rightTicks++; else rightTicks--; }

// ============================================================
// MOTORS (same logic as working code)
// ============================================================
void setMotor(int in1, int in2, int speed) {
  speed = constrain(speed, -255, 255);
  if (speed >= 0) { ledcWrite(in1, 0);      ledcWrite(in2, speed); }
  else            { ledcWrite(in1, -speed); ledcWrite(in2, 0);     }
}
void drive(int rightSpeed, int leftSpeed) {
  setMotor(AIN1, AIN2, rightSpeed);
  setMotor(BIN1, BIN2, leftSpeed);
  moving = (rightSpeed != 0 || leftSpeed != 0);
}
void stopRobot() { drive(0, 0); }

// ============================================================
// ODOMETRY
// ============================================================
void updateOdometry() {
  noInterrupts();
  long l = leftTicks, r = rightTicks;
  interrupts();

  float dL = (l - lastL) * LEFT_SIGN  * MM_PER_COUNT;
  float dR = (r - lastR) * RIGHT_SIGN * MM_PER_COUNT;
  lastL = l; lastR = r;

  float dCenter = (dL + dR) / 2.0;
  float dTheta  = (dR - dL) / WHEEL_BASE_MM;   // left turn = positive

  poseX += dCenter * cos(poseTheta + dTheta / 2.0);
  poseY += dCenter * sin(poseTheta + dTheta / 2.0);
  poseTheta += dTheta;
}

// ============================================================
// OCCUPANCY GRID
// ============================================================
void bumpCell(float xmm, float ymm, int delta) {
  int gx = GRID_N / 2 + (int)floor(xmm / CELL_MM);
  int gy = GRID_N / 2 + (int)floor(ymm / CELL_MM);
  if (gx < 0 || gx >= GRID_N || gy < 0 || gy >= GRID_N) return;
  int idx = gy * GRID_N + gx;
  int v = grid[idx] + delta;
  grid[idx] = constrain(v, -100, 100);
}

// Trace one sensor beam: cells along it are free, the end cell is occupied
void integrateRay(float sensorAngle, int mm) {
  if (mm <= 0) return;
  bool hit = (mm < MAX_RANGE_MM);
  int maxD = hit ? mm : MAX_RANGE_MM;
  float a = poseTheta + sensorAngle;
  float ca = cos(a), sa = sin(a);

  for (int d = CELL_MM / 2; d < maxD; d += CELL_MM / 2) {
    bumpCell(poseX + d * ca, poseY + d * sa, -3);   // free
  }
  if (hit) bumpCell(poseX + mm * ca, poseY + mm * sa, +15);   // occupied
}

void readSensorsAndMap() {
  if (okLeft) {
    int v = sensorLeft.readRangeContinuousMillimeters();
    if (!sensorLeft.timeoutOccurred()) integrateRay(ANGLE_LEFT, v + offsetLeft);
  }
  if (okCenter) {
    int v = sensorCenter.readRangeContinuousMillimeters();
    if (!sensorCenter.timeoutOccurred()) integrateRay(ANGLE_CENTER, v + offsetCenter);
  }
  if (okRight) {
    int v = sensorRight.readRangeContinuousMillimeters();
    if (!sensorRight.timeoutOccurred()) integrateRay(ANGLE_RIGHT, v + offsetRight);
  }
}

void resetMap() {
  memset(grid, 0, sizeof(grid));
  poseX = poseY = poseTheta = 0;
  noInterrupts();
  lastL = leftTicks; lastR = rightTicks;
  interrupts();
}

// ============================================================
// SENSOR SETUP (same sequence as working code, but doesn't halt on failure)
// ============================================================
bool initOne(VL53L0X &s, int xshut, uint8_t addr, const char* name) {
  digitalWrite(xshut, HIGH);
  delay(10);
  s.setTimeout(500);
  if (!s.init()) { Serial.print(name); Serial.println(" sensor FAILED"); return false; }
  s.setAddress(addr);
  s.startContinuous();
  Serial.print(name); Serial.println(" sensor OK");
  return true;
}

void setupSensors() {
  Wire.begin(21, 22);
  pinMode(XSHUT_LEFT, OUTPUT); pinMode(XSHUT_RIGHT, OUTPUT); pinMode(XSHUT_CENTER, OUTPUT);
  digitalWrite(XSHUT_LEFT, LOW); digitalWrite(XSHUT_RIGHT, LOW); digitalWrite(XSHUT_CENTER, LOW);
  delay(10);
  okLeft   = initOne(sensorLeft,   XSHUT_LEFT,   ADDR_LEFT,   "LEFT");
  okRight  = initOne(sensorRight,  XSHUT_RIGHT,  ADDR_RIGHT,  "RIGHT");
  okCenter = initOne(sensorCenter, XSHUT_CENTER, ADDR_CENTER, "CENTER");
}

// ============================================================
// WEB PAGE: drive pad + live map
// ============================================================
const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>ESP32 Robot</title>
<style>
  * { box-sizing: border-box; -webkit-user-select: none; user-select: none;
      -webkit-touch-callout: none; touch-action: none; }
  body { margin: 0; background: #111; color: #eee; font-family: sans-serif;
         text-align: center; padding: 12px; }
  h1 { font-size: 18px; margin: 6px 0 10px; }
  canvas { background: #222; width: 100%; max-width: 340px; image-rendering: pixelated;
           border: 1px solid #444; }
  .pad { display: grid; grid-template-columns: repeat(3, 1fr); gap: 8px;
         max-width: 340px; margin: 12px auto 0; }
  button { height: 76px; font-size: 28px; border: none; border-radius: 14px;
           background: #2d6cdf; color: #fff; }
  button:active { background: #7fa8ff; }
  button.stop { background: #d33; font-size: 20px; }
  .row { max-width: 340px; margin: 14px auto 0; }
  input[type=range] { width: 100%; height: 36px; }
  .small { background: #444; height: 44px; font-size: 16px; width: 100%; margin-top: 8px; }
</style>
</head>
<body>
  <h1>ESP32 Robot - Live Map</h1>
  <canvas id="map" width="320" height="320"></canvas>

  <div class="pad">
    <button data-d="FL">&#8598;</button>
    <button data-d="F">&#9650;</button>
    <button data-d="FR">&#8599;</button>
    <button data-d="L">&#9664;</button>
    <button class="stop" id="stopBtn">STOP</button>
    <button data-d="R">&#9654;</button>
    <button data-d="BL">&#8601;</button>
    <button data-d="B">&#9660;</button>
    <button data-d="BR">&#8600;</button>
  </div>

  <div class="row">
    Speed: <span id="spdVal">200</span>
    <input type="range" id="spd" min="100" max="255" value="200">
    <button class="small" id="resetBtn">Reset map</button>
  </div>

<script>
  const N = 40, S = 8;
  const cv = document.getElementById('map');
  const ctx = cv.getContext('2d');
  const spd = document.getElementById('spd');
  const spdVal = document.getElementById('spdVal');
  spd.oninput = () => { spdVal.textContent = spd.value; };

  // ---- driving ----
  let timer = null;
  function send(d) { fetch('/move?d=' + d + '&s=' + spd.value).catch(() => {}); }
  function start(d) { send(d); clearInterval(timer); timer = setInterval(() => send(d), 120); }
  function stop() { clearInterval(timer); timer = null; fetch('/move?d=S&s=0').catch(() => {}); }
  document.querySelectorAll('button[data-d]').forEach(b => {
    const d = b.dataset.d;
    b.addEventListener('pointerdown', e => { e.preventDefault(); start(d); });
    b.addEventListener('pointerup', stop);
    b.addEventListener('pointercancel', stop);
    b.addEventListener('pointerleave', stop);
    b.addEventListener('contextmenu', e => e.preventDefault());
  });
  document.getElementById('stopBtn').addEventListener('pointerdown', stop);
  document.getElementById('resetBtn').addEventListener('click', () => fetch('/reset'));
  window.addEventListener('blur', stop);

  // ---- map drawing ----
  async function drawMap() {
    try {
      const t = await (await fetch('/map')).text();
      const nl = t.indexOf('\n');
      const [cx, cy, th] = t.slice(0, nl).split(',').map(Number);
      const cells = t.slice(nl + 1);
      ctx.fillStyle = '#222';
      ctx.fillRect(0, 0, N * S, N * S);
      for (let gy = 0; gy < N; gy++) {
        for (let gx = 0; gx < N; gx++) {
          const c = cells[gy * N + gx];
          if (c === '0') continue;
          ctx.fillStyle = (c === '2') ? '#e33' : '#ccc';
          ctx.fillRect(gx * S, (N - 1 - gy) * S, S, S);   // +Y is up on screen
        }
      }
      // robot
      const px = cx * S, py = (N - cy) * S;
      ctx.save();
      ctx.translate(px, py);
      ctx.rotate(-th);
      ctx.fillStyle = '#3c6';
      ctx.beginPath();
      ctx.moveTo(10, 0); ctx.lineTo(-7, 6); ctx.lineTo(-7, -6);
      ctx.closePath(); ctx.fill();
      ctx.restore();
    } catch (e) {}
  }
  setInterval(drawMap, 500);
</script>
</body>
</html>
)rawliteral";

// ============================================================
// WEB HANDLERS
// ============================================================
void handleRoot() { server.send_P(200, "text/html", PAGE); }

void handleMove() {
  String d = server.arg("d");
  int s = constrain(server.arg("s").toInt(), 0, 255);
  int slow = s * 4 / 10;
  lastCmd = millis();

  if      (d == "F")  drive( s,     s);
  else if (d == "B")  drive(-s,    -s);
  else if (d == "L")  drive( s,    -s);
  else if (d == "R")  drive(-s,     s);
  else if (d == "FL") drive( s,     slow);
  else if (d == "FR") drive( slow,  s);
  else if (d == "BL") drive(-slow, -s);
  else if (d == "BR") drive(-s,    -slow);
  else                stopRobot();

  server.send(200, "text/plain", "OK");
}

// Line 1: robot x_cell,y_cell,theta_rad   Then GRID_N*GRID_N chars: 0=unknown 1=free 2=occupied
void handleMap() {
  String out;
  out.reserve(GRID_N * GRID_N + 40);
  out += String(GRID_N / 2.0 + poseX / CELL_MM, 2); out += ',';
  out += String(GRID_N / 2.0 + poseY / CELL_MM, 2); out += ',';
  out += String(poseTheta, 3); out += '\n';
  for (int i = 0; i < GRID_N * GRID_N; i++) {
    int8_t v = grid[i];
    out += (v > 20) ? '2' : (v < -10) ? '1' : '0';
  }
  server.send(200, "text/plain", out);
}

void handleReset() { resetMap(); server.send(200, "text/plain", "OK"); }

// ============================================================
// SETUP / LOOP
// ============================================================
void setup() {
  Serial.begin(115200);

  ledcAttach(AIN1, PWM_FREQ, PWM_BITS);
  ledcAttach(AIN2, PWM_FREQ, PWM_BITS);
  ledcAttach(BIN1, PWM_FREQ, PWM_BITS);
  ledcAttach(BIN2, PWM_FREQ, PWM_BITS);
  stopRobot();

  pinMode(ENC_L_A, INPUT); pinMode(ENC_L_B, INPUT);
  pinMode(ENC_R_A, INPUT); pinMode(ENC_R_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENC_L_A), isrLeft,  RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_R_A), isrRight, RISING);

  setupSensors();
  resetMap();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("Open in browser: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.on("/map", handleMap);
  server.on("/reset", handleReset);
  server.begin();
}

void loop() {
  server.handleClient();

  // Failsafe: stop if the phone stops sending commands
  if (moving && (millis() - lastCmd > COMMAND_TIMEOUT)) {
    stopRobot();
  }

  // Odometry + mapping at ~10 Hz
  if (millis() - lastSense >= 100) {
    lastSense = millis();
    updateOdometry();
    readSensorsAndMap();

    // Debug: watch ticks while pushing the robot forward by hand
    // Serial.printf("L=%ld R=%ld x=%.0f y=%.0f th=%.2f\n", leftTicks, rightTicks, poseX, poseY, poseTheta);
  }
}