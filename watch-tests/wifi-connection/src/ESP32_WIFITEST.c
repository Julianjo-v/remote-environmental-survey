#include <Arduino.h>
#include <WiFi.h>
#include "Arduino_GFX_Library.h"
#include "pin_config.h"
#include "HWCDC.h"

HWCDC USBSerial;

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
  LCD_CS,
  LCD_SCLK,
  LCD_SDIO0,
  LCD_SDIO1,
  LCD_SDIO2,
  LCD_SDIO3
);

Arduino_GFX *gfx = new Arduino_CO5300(
  bus,
  LCD_RESET,
  0,
  LCD_WIDTH,
  LCD_HEIGHT,
  22,  // Required AMOLED column offset
  0, 0, 0
);

constexpr uint16_t COLOR_BLACK  = 0x0000;
constexpr uint16_t COLOR_WHITE  = 0xFFFF;
constexpr uint16_t COLOR_CYAN   = 0x07FF;
constexpr uint16_t COLOR_GREEN  = 0x07E0;
constexpr uint16_t COLOR_YELLOW = 0xFFE0;
constexpr uint16_t COLOR_RED    = 0xF800;
constexpr uint16_t COLOR_GREY   = 0x8410;

constexpr int NETWORKS_PER_PAGE = 7;
constexpr unsigned long PAGE_TIME_MS = 5000;

int networkCount = -1;
int currentPage = 0;
unsigned long pageStartedAt = 0;

void drawCentered(
  const String &text,
  int y,
  uint8_t size,
  uint16_t color
) {
  int textWidth = text.length() * 6 * size;
  int x = (LCD_WIDTH - textWidth) / 2;

  if (x < 30) {
    x = 30;
  }

  gfx->setTextSize(size);
  gfx->setTextColor(color);
  gfx->setCursor(x, y);
  gfx->print(text);
}

int totalPageCount() {
  if (networkCount <= 0) {
    return 1;
  }

  return (networkCount + NETWORKS_PER_PAGE - 1)
         / NETWORKS_PER_PAGE;
}

String shortenedSSID(int index) {
  String name = WiFi.SSID(index);

  if (name.length() == 0) {
    name = "<hidden>";
  }

  if (name.length() > 22) {
    name = name.substring(0, 19) + "...";
  }

  return name;
}

uint16_t rssiColor(int rssi) {
  if (rssi >= -60) {
    return COLOR_GREEN;
  }

  if (rssi >= -75) {
    return COLOR_YELLOW;
  }

  return COLOR_RED;
}

void drawScanningScreen() {
  gfx->fillScreen(COLOR_BLACK);

  drawCentered("Scanning", 215, 3, COLOR_CYAN);
  drawCentered("Wi-Fi...", 255, 3, COLOR_WHITE);
}

void drawResultsPage() {
  gfx->fillScreen(COLOR_BLACK);

  drawCentered("Nearby Wi-Fi", 48, 2, COLOR_CYAN);

  String status = "Found ";
  status += networkCount < 0 ? 0 : networkCount;
  status += "  Page ";
  status += currentPage + 1;
  status += "/";
  status += totalPageCount();

  drawCentered(status, 80, 2, COLOR_WHITE);

  gfx->drawFastHLine(
    30,
    110,
    LCD_WIDTH - 60,
    COLOR_GREY
  );

  if (networkCount < 0) {
    drawCentered("Scan failed", 220, 2, COLOR_RED);
    drawCentered("Retrying soon...", 255, 2, COLOR_WHITE);
    return;
  }

  if (networkCount == 0) {
    drawCentered(
      "No networks found",
      220,
      2,
      COLOR_YELLOW
    );

    drawCentered(
      "Retrying soon...",
      255,
      2,
      COLOR_WHITE
    );

    return;
  }

  int firstNetwork =
    currentPage * NETWORKS_PER_PAGE;

  int lastNetwork =
    firstNetwork + NETWORKS_PER_PAGE;

  if (lastNetwork > networkCount) {
    lastNetwork = networkCount;
  }

  int y = 125;

  for (int i = firstNetwork; i < lastNetwork; i++) {
    String name = shortenedSSID(i);
    int rssi = WiFi.RSSI(i);

    gfx->setTextSize(2);
    gfx->setTextColor(COLOR_WHITE);
    gfx->setCursor(30, y);

    gfx->print(i + 1);
    gfx->print(". ");
    gfx->println(name);

    gfx->setTextColor(rssiColor(rssi));
    gfx->setCursor(60, y + 18);

    gfx->print(rssi);
    gfx->print(" dBm  CH ");
    gfx->println(WiFi.channel(i));

    y += 45;
  }

  gfx->setTextSize(1);
  gfx->setTextColor(COLOR_GREY);
  gfx->setCursor(30, 466);

  if (currentPage + 1 < totalPageCount()) {
    gfx->println("Next page in 5 seconds");
  } else {
    gfx->println("Rescan in 5 seconds");
  }
}

void printResultsToSerial() {
  USBSerial.print("Networks found: ");
  USBSerial.println(networkCount);

  for (int i = 0; i < networkCount; i++) {
    USBSerial.print(i + 1);
    USBSerial.print(". ");
    USBSerial.print(WiFi.SSID(i));
    USBSerial.print(" | ");
    USBSerial.print(WiFi.RSSI(i));
    USBSerial.print(" dBm | Channel ");
    USBSerial.println(WiFi.channel(i));
  }
}

void scanForNetworks() {
  WiFi.scanDelete();

  drawScanningScreen();

  USBSerial.println();
  USBSerial.println("Scanning for Wi-Fi networks...");

  // Blocking scan that includes hidden networks.
  networkCount = WiFi.scanNetworks(false, true);

  if (networkCount >= 0) {
    printResultsToSerial();
  } else {
    USBSerial.print("Scan failed with code: ");
    USBSerial.println(networkCount);
  }

  currentPage = 0;
  drawResultsPage();
  pageStartedAt = millis();
}

void setup() {
  USBSerial.begin(115200);
  delay(500);

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  if (!gfx->begin()) {
    USBSerial.println(
      "Display initialization failed"
    );
  }

  gfx->fillScreen(COLOR_BLACK);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(false, false);
  delay(200);

  scanForNetworks();
}

void loop() {
  if (millis() - pageStartedAt < PAGE_TIME_MS) {
    delay(10);
    return;
  }

  if (currentPage + 1 < totalPageCount()) {
    currentPage++;
    drawResultsPage();
    pageStartedAt = millis();
  } else {
    scanForNetworks();
  }
}
