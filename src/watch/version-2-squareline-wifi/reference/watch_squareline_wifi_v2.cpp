#include <Wire.h>
#include <Arduino.h>
#include <WiFi.h>
#include "pin_config.h"
#include <lvgl.h>

#include "Arduino_GFX_Library.h"
#include "Arduino_DriveBus_Library.h"
#include "lv_conf.h"
#include "ui.h"

#include "HWCDC.h"
HWCDC USBSerial;

uint32_t screenWidth;
uint32_t screenHeight;
uint32_t bufSize;
lv_display_t *disp;
lv_color_t *disp_draw_buf;

bool wifiWanted = false;
bool wifiStateChangePending = false;
bool wifiScanRunning = false;

constexpr size_t MAX_WIFI_NETWORKS = 24;

struct WifiNetworkEntry {
  char ssid[33];
  int32_t rssi;
  int32_t channel;
};

WifiNetworkEntry wifiNetworks[MAX_WIFI_NETWORKS];
size_t wifiNetworkCount = 0;

#define DIRECT_RENDER_MODE

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
  LCD_CS /* CS */, LCD_SCLK /* SCK */, LCD_SDIO0 /* SDIO0 */, LCD_SDIO1 /* SDIO1 */,
  LCD_SDIO2 /* SDIO2 */, LCD_SDIO3 /* SDIO3 */);

Arduino_GFX *gfx = new Arduino_CO5300(bus, LCD_RESET /* RST */,
                                      0 /* rotation */, LCD_WIDTH, LCD_HEIGHT,
                                      22 /* col_offset1 */,
                                      0 /* row_offset1 */,
                                      0 /* col_offset2 */,
                                      0 /* row_offset2 */);


std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
  std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> FT3168(new Arduino_FT3x68(IIC_Bus, FT3168_DEVICE_ADDRESS,
                                                       DRIVEBUS_DEFAULT_VALUE, TP_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void) {
  FT3168->IIC_Interrupt_Flag = true;
}

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf) {
  LV_UNUSED(level);
  USBSerial.println(buf);
  USBSerial.flush();
}
#endif

uint32_t millis_cb(void) {
  return millis();
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
#ifndef DIRECT_RENDER_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif  // #ifndef DIRECT_RENDER_MODE

  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
  int32_t touchX = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
  int32_t touchY = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

  if (FT3168->IIC_Interrupt_Flag == true) {
    FT3168->IIC_Interrupt_Flag = false;
    data->state = LV_INDEV_STATE_PR;

    /*Set the coordinates*/
    data->point.x = touchX;
    data->point.y = touchY;

    USBSerial.print("Data x ");
    USBSerial.print(touchX);

    USBSerial.print("Data y ");
    USBSerial.println(touchY);
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void rounder_event_cb(lv_event_t *e) {
  lv_area_t *area = (lv_area_t *)lv_event_get_param(e);
  uint16_t x1 = area->x1;
  uint16_t x2 = area->x2;

  uint16_t y1 = area->y1;
  uint16_t y2 = area->y2;

  // round the start of coordinate down to the nearest 2M number
  area->x1 = (x1 >> 1) << 1;
  area->y1 = (y1 >> 1) << 1;
  // round the end of coordinate up to the nearest 2N+1 number
  area->x2 = ((x2 >> 1) << 1) + 1;
  area->y2 = ((y2 >> 1) << 1) + 1;
}

void wifiSwitchEvent(lv_event_t *event) {
  lv_obj_t *wifiSwitch =
    (lv_obj_t *)lv_event_get_target(event);

  wifiWanted =
    lv_obj_has_state(wifiSwitch, LV_STATE_CHECKED);
  wifiStateChangePending = true;
}

void wifiNetworkClicked(lv_event_t *event) {
  WifiNetworkEntry *network =
    (WifiNetworkEntry *)lv_event_get_user_data(event);

  if (network == nullptr) {
    return;
  }

  char status[48];
  snprintf(status, sizeof(status),
           "SELECTED: %.32s", network->ssid);
  lv_label_set_text(ui_SCANNINGFOUND, status);

  USBSerial.print("Selected network: ");
  USBSerial.println(network->ssid);
}

void clearWifiList() {
  wifiNetworkCount = 0;
  lv_obj_clean(ui_WIFILIST);
}

void saveWifiResults(int16_t detectedCount) {
  size_t availableCount = (size_t)detectedCount;
  wifiNetworkCount =
    availableCount < MAX_WIFI_NETWORKS
      ? availableCount
      : MAX_WIFI_NETWORKS;

  for (size_t i = 0; i < wifiNetworkCount; i++) {
    String name = WiFi.SSID(i);

    if (name.length() == 0) {
      name = "<hidden>";
    }

    snprintf(wifiNetworks[i].ssid,
             sizeof(wifiNetworks[i].ssid),
             "%s", name.c_str());
    wifiNetworks[i].rssi = WiFi.RSSI(i);
    wifiNetworks[i].channel = WiFi.channel(i);
  }
}

void rebuildWifiList() {
  lv_obj_clean(ui_WIFILIST);

  for (size_t i = 0; i < wifiNetworkCount; i++) {
    lv_obj_t *button = lv_button_create(ui_WIFILIST);
    lv_obj_set_width(button, LV_PCT(100));
    lv_obj_set_height(button, 82);
    lv_obj_set_flex_grow(button, 0);
    lv_obj_set_style_pad_left(button, 12, 0);
    lv_obj_set_style_pad_right(button, 12, 0);
    lv_obj_set_style_pad_top(button, 8, 0);
    lv_obj_set_style_pad_bottom(button, 8, 0);
    lv_obj_add_event_cb(button,
                        wifiNetworkClicked,
                        LV_EVENT_CLICKED,
                        &wifiNetworks[i]);

    lv_obj_t *label = lv_label_create(button);
    lv_obj_set_width(label, LV_PCT(100));
    lv_obj_set_height(label, LV_SIZE_CONTENT);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_LEFT, 0);
    lv_label_set_text_fmt(label,
                          "%s\n%ld dBm  CH %ld",
                          wifiNetworks[i].ssid,
                          (long)wifiNetworks[i].rssi,
                          (long)wifiNetworks[i].channel);
    lv_obj_center(label);
  }

  lv_obj_scroll_to_y(ui_WIFILIST, 0, LV_ANIM_OFF);
}

void startWifiScan() {
  WiFi.scanDelete();
  lv_label_set_text(ui_SCANNINGFOUND, "SEARCHING...");

  // Asynchronous scan that includes hidden 2.4 GHz networks.
  int16_t result = WiFi.scanNetworks(true, true);

  if (result == WIFI_SCAN_FAILED) {
    wifiScanRunning = false;
    lv_label_set_text(ui_SCANNINGFOUND, "SCAN FAILED");
    return;
  }

  wifiScanRunning = true;
}

void printWifiResults(int16_t networkCount) {
  USBSerial.print("Networks found: ");
  USBSerial.println(networkCount);

  for (int i = 0; i < networkCount; i++) {
    String name = WiFi.SSID(i);

    if (name.length() == 0) {
      name = "<hidden>";
    }

    USBSerial.print(i + 1);
    USBSerial.print(". ");
    USBSerial.print(name);
    USBSerial.print(" | ");
    USBSerial.print(WiFi.RSSI(i));
    USBSerial.print(" dBm | Channel ");
    USBSerial.println(WiFi.channel(i));
  }
}

void serviceWifiScanner() {
  if (wifiStateChangePending) {
    wifiStateChangePending = false;

    if (!wifiWanted) {
      wifiScanRunning = false;
      WiFi.scanDelete();
      WiFi.mode(WIFI_OFF);
      clearWifiList();
      lv_obj_add_flag(ui_WIFILIST, LV_OBJ_FLAG_HIDDEN);
      lv_label_set_text(ui_Label2, "DISABLED");
      lv_label_set_text(ui_SCANNINGFOUND, "");
      return;
    }

    clearWifiList();
    lv_obj_remove_flag(ui_WIFILIST, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_Label2, "ENABLED");
    lv_label_set_text(ui_SCANNINGFOUND, "SEARCHING...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(false, false);
    startWifiScan();
  }

  if (!wifiWanted) {
    return;
  }

  if (wifiScanRunning) {
    int16_t networkCount = WiFi.scanComplete();

    if (networkCount == WIFI_SCAN_RUNNING) {
      return;
    }

    wifiScanRunning = false;

    if (networkCount == WIFI_SCAN_FAILED) {
      lv_label_set_text(ui_SCANNINGFOUND, "SCAN FAILED");
      return;
    }

    char status[24];
    snprintf(status, sizeof(status), "FOUND %d", networkCount);
    lv_label_set_text(ui_SCANNINGFOUND, status);
    saveWifiResults(networkCount);
    rebuildWifiList();
    printWifiResults(networkCount);

    if ((size_t)networkCount > MAX_WIFI_NETWORKS) {
      USBSerial.print("UI list limited to ");
      USBSerial.print(MAX_WIFI_NETWORKS);
      USBSerial.println(" scan entries.");
    }

    WiFi.scanDelete();
    return;
  }
}

void setup() {
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif

  USBSerial.begin(115200);
  // USBSerial.setDebugOutput(true);
  // while(!USBSerial);
  USBSerial.println("Arduino_GFX LVGL_Arduino_v9 example ");
  String LVGL_Arduino = String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  USBSerial.println(LVGL_Arduino);

  // Init Display
  if (!gfx->begin()) {
    USBSerial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(RGB565_BLACK);

  Wire.begin(IIC_SDA, IIC_SCL);

  while (FT3168->begin() == false) {
    USBSerial.println("FT3168 initialization fail");
    delay(2000);
  }
  USBSerial.println("FT3168 initialization successfully");

  FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_POWER_MODE,
                                 FT3168->Arduino_IIC_Touch::Device_Mode::TOUCH_POWER_MONITOR);

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(millis_cb);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  screenWidth = gfx->width();
  screenHeight = gfx->height();

#ifdef DIRECT_RENDER_MODE
  bufSize = screenWidth * screenHeight;
#else
  bufSize = screenWidth * 50;
#endif

#ifdef ESP32
#if defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL))
  disp_draw_buf = (lv_color_t *)gfx->getFramebuffer();
#else   // !(defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)))
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf) {
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
  }
#endif  // !(defined(DIRECT_RENDER_MODE) && (defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)))
#else   // !ESP32
  USBSerial.println("LVGL disp_draw_buf heap_caps_malloc failed! malloc again...");
  disp_draw_buf = (lv_color_t *)malloc(bufSize * 2);
#endif  // !ESP32
  if (!disp_draw_buf) {
    USBSerial.println("LVGL disp_draw_buf allocate failed!");
  } else {
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
#ifdef DIRECT_RENDER_MODE
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_DIRECT);
#else
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);
    lv_display_add_event_cb(disp, rounder_event_cb, LV_EVENT_INVALIDATE_AREA, NULL);

    ui_init();

    lv_obj_clear_state(ui_Switch1, LV_STATE_CHECKED);
    lv_label_set_text(ui_Label2, "DISABLED");
    lv_label_set_text(ui_SCANNINGFOUND, "");
    lv_obj_add_flag(ui_WIFILIST, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(
      ui_Switch1,
      wifiSwitchEvent,
      LV_EVENT_VALUE_CHANGED,
      nullptr);

    WiFi.mode(WIFI_OFF);
  }

  USBSerial.println("Setup done");
}

void loop() {
  lv_task_handler(); /* let the GUI do its work */
  serviceWifiScanner();

#ifdef DIRECT_RENDER_MODE
#if defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL)
  gfx->flush();
#else   // !(defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL))
  gfx->draw16bitRGBBitmap(0, 0, (uint16_t *)disp_draw_buf, screenWidth, screenHeight);
#endif  // !(defined(CANVAS) || defined(RGB_PANEL) || defined(DSI_PANEL))
#else   // !DIRECT_RENDER_MODE
#ifdef CANVAS
  gfx->flush();
#endif
#endif  // !DIRECT_RENDER_MODE

  delay(5);
}
