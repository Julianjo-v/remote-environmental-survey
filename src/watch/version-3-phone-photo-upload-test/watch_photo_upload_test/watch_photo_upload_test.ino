#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_heap_caps.h>
#include "Arduino_GFX_Library.h"
#include "pin_config.h"

// Standalone transfer test. The phone joins the watch directly; no SD card.
constexpr char AP_NAME[] = "WatchPhotoTest";
constexpr char AP_PASSWORD[] = "REPLACE_WITH_PRIVATE_PASSWORD";
constexpr uint16_t MAX_W = 320;
constexpr uint16_t MAX_H = 380;
constexpr size_t MAX_PACKET = 8 + size_t(MAX_W) * MAX_H * 2;

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS, LCD_SCLK, LCD_SDIO0, LCD_SDIO1, LCD_SDIO2, LCD_SDIO3);
Arduino_GFX *gfx = new Arduino_CO5300(
    bus, LCD_RESET, 0, LCD_WIDTH, LCD_HEIGHT, 22, 0, 0, 0);
WebServer server(80);

uint8_t *savedPacket = nullptr;
uint8_t *incomingPacket = nullptr;
size_t incomingLength = 0;
uint16_t savedW = 0;
uint16_t savedH = 0;
bool incomingError = false;
bool uploadReadyToDraw = false;
bool apReady = false;

const char UPLOAD_PAGE[] PROGMEM = R"HTML(
<!doctype html><html lang="en"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Watch photo test</title>
<style>body{font:17px system-ui;background:#101828;color:#fff;max-width:500px;margin:24px auto;padding:0 18px}
button,input{font:inherit;margin:12px 0}button{padding:12px 18px;background:#1688e8;color:white;border:0;border-radius:8px}
img{display:block;max-width:100%;max-height:380px;margin:15px 0;border:1px solid #667}</style>
<h2>Send a photo to the watch</h2><p>Choose a JPEG, PNG, or WebP photo. Your phone resizes it for the watch before sending.</p>
<input id="file" type="file" accept="image/jpeg,image/png,image/webp"><br>
<button id="send">Show on watch</button><p id="status">Ready.</p><img id="preview" hidden>
<script>
const file=document.getElementById('file'),status=document.getElementById('status'),preview=document.getElementById('preview');
document.getElementById('send').onclick=async()=>{
  if(!file.files.length){status.textContent='Choose a photo first.';return;}
  try{
    status.textContent='Preparing photo...';
    const photo=file.files[0];
    if(!['image/jpeg','image/png','image/webp'].includes(photo.type))throw Error('Choose JPEG, PNG, or WebP.');
    const bitmap=await createImageBitmap(photo);
    const scale=Math.min(1,320/bitmap.width,380/bitmap.height);
    const w=Math.max(1,Math.floor(bitmap.width*scale)),h=Math.max(1,Math.floor(bitmap.height*scale));
    const canvas=document.createElement('canvas');canvas.width=w;canvas.height=h;
    const ctx=canvas.getContext('2d',{willReadFrequently:true});ctx.fillStyle='#000';ctx.fillRect(0,0,w,h);
    ctx.drawImage(bitmap,0,0,w,h);bitmap.close();
    const rgba=ctx.getImageData(0,0,w,h).data;
    const packet=new ArrayBuffer(8+w*h*2),view=new DataView(packet),bytes=new Uint8Array(packet);
    bytes.set([87,73,77,71],0);view.setUint16(4,w,true);view.setUint16(6,h,true);
    for(let i=0;i<w*h;i++){
      const j=i*4,a=rgba[j+3];
      const r=Math.round((rgba[j]*a)/255),g=Math.round((rgba[j+1]*a)/255),b=Math.round((rgba[j+2]*a)/255);
      view.setUint16(8+i*2,((r>>3)<<11)|((g>>2)<<5)|(b>>3),true);
    }
    preview.src=canvas.toDataURL('image/png');preview.hidden=false;
    const form=new FormData();form.append('photo',new Blob([packet],{type:'application/octet-stream'}),'photo.rgb565');
    status.textContent=`Uploading ${w} x ${h} photo...`;
    const response=await fetch('/upload',{method:'POST',body:form});
    const message=await response.text();if(!response.ok)throw Error(message);
    status.textContent=message;
  }catch(e){status.textContent='Upload failed: '+e.message;}
};
</script></html>
)HTML";

void drawHeader() {
  gfx->fillScreen(0x0000);
  gfx->setTextColor(0xFFFF);
  gfx->setTextSize(2);
  gfx->setCursor(12, 12);
  gfx->println("Photo upload test");
  gfx->setTextSize(1);
  gfx->setCursor(12, 42);
  gfx->print("Wi-Fi: "); gfx->println(AP_NAME);
  gfx->setCursor(12, 56);
  gfx->print("Password: "); gfx->println(AP_PASSWORD);
  gfx->setCursor(12, 70);
  gfx->print("Open: http://");
  gfx->println(apReady ? WiFi.softAPIP().toString() : String("AP failed"));
}

void drawPhoto() {
  drawHeader();
  if(!savedPacket) {
    gfx->setCursor(12, 115);
    gfx->println("Waiting for a photo...");
    return;
  }
  const int16_t x=(LCD_WIDTH-savedW)/2;
  const int16_t y=94+(380-savedH)/2;
  gfx->draw16bitRGBBitmap(x,y,reinterpret_cast<uint16_t *>(savedPacket+8),savedW,savedH);
}

uint16_t read16(const uint8_t *p) { return uint16_t(p[0]) | (uint16_t(p[1])<<8); }

void handleUploadData() {
  HTTPUpload &upload=server.upload();
  if(upload.status==UPLOAD_FILE_START) {
    if(incomingPacket) { heap_caps_free(incomingPacket); incomingPacket=nullptr; }
    incomingPacket=static_cast<uint8_t *>(heap_caps_malloc(MAX_PACKET,MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT));
    incomingLength=0;
    incomingError=(incomingPacket==nullptr);
    return;
  }
  if(upload.status==UPLOAD_FILE_WRITE) {
    if(incomingError) return;
    if(incomingLength+upload.currentSize>MAX_PACKET) { incomingError=true; return; }
    memcpy(incomingPacket+incomingLength,upload.buf,upload.currentSize);
    incomingLength+=upload.currentSize;
    return;
  }
  if(upload.status==UPLOAD_FILE_ABORTED) {
    incomingError=true;
    if(incomingPacket) { heap_caps_free(incomingPacket); incomingPacket=nullptr; }
  }
}

void finishUpload() {
  bool valid=!incomingError && incomingPacket && incomingLength>=8;
  uint16_t w=0,h=0;
  if(valid) {
    valid=memcmp(incomingPacket,"WIMG",4)==0;
    if(valid) { w=read16(incomingPacket+4); h=read16(incomingPacket+6); }
    valid=valid && w>0 && h>0 && w<=MAX_W && h<=MAX_H &&
          incomingLength==8+size_t(w)*h*2;
  }
  if(!valid) {
    if(incomingPacket) { heap_caps_free(incomingPacket); incomingPacket=nullptr; }
    server.send(400,"text/plain","Invalid photo data or insufficient PSRAM.");
    return;
  }
  if(savedPacket) heap_caps_free(savedPacket);
  savedPacket=incomingPacket; incomingPacket=nullptr;
  savedW=w; savedH=h; uploadReadyToDraw=true;
  server.send(200,"text/plain","Photo received. Check the watch screen.");
}

void setup() {
  Serial.begin(115200);
#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif
  if(!gfx->begin()) {
    Serial.println("Display init failed");
    while(true) delay(1000);
  }
  WiFi.mode(WIFI_AP);
  apReady=WiFi.softAP(AP_NAME,AP_PASSWORD);
  drawPhoto();
  server.on("/",HTTP_GET,[](){server.send_P(200,"text/html",UPLOAD_PAGE);});
  server.on("/upload",HTTP_POST,finishUpload,handleUploadData);
  server.begin();
}

void loop() {
  server.handleClient();
  if(uploadReadyToDraw) { uploadReadyToDraw=false; drawPhoto(); }
  delay(2);
}
