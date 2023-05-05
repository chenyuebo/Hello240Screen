#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <OneButton.h>
#include <Ticker.h>
#include <TJpg_Decoder.h>

#include "Page.h"
#include "SplashPage.h"
#include "MonitorPage.h"
#include "WifiConnectPage.h"
#include "AstronautPage.h"
#include "PhotoAlbumPage.h"
#include "WeatherPage.h"
#include "VncPage.h"

unsigned char buff[1024];
// 建立WiFi客户端对象，对象名称client
WiFiClient wifiClient;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
#define LCD_BL_PIN 12 // LCD背光引脚
uint16_t bgColor = 0x0000;

Ticker ticker;
OneButton btn1 = OneButton(0, true, true);
OneButton btn2 = OneButton(4, true, true);
OneButton btn3 = OneButton(5, true, true);

WiFiUDP ntpUDP;
unsigned long epochTime;                                                   // 当前的时间，距1970-01-01的秒数
unsigned long timeOffset = 3600 * 8;                                       // 北京时间+8
unsigned long updateInterval = 5 * 60 * 1000;                              // 每5分钟同步一次服务器时间
NTPClient ntpClient(ntpUDP, "time.apple.com", timeOffset, updateInterval); // 时区8小时
unsigned long lastPrintTime = 0;

// VNC投屏
WiFiServer server(8080);

SplashPage splashPage = SplashPage(tft);
MonitorPage monitorPage = MonitorPage(tft, sprite, wifiClient, ntpClient);
WifiConnectPage wifiPage = WifiConnectPage(tft);
// AstronautPage astronaut = AstronautPage(tft);
PhotoAlbumPage photoPage = PhotoAlbumPage(tft);
WeatherPage weatherPage = WeatherPage(tft, sprite, wifiClient, ntpClient);
VncPage vncPage = VncPage(tft, server);

int currentPageIndex = -1;
int nextPageIndex = 0;

void showClock();
void initTJpgDec();
void btnTick();
void handleClick1();
void handleClick2();
void handleClick3();
Page *getPage(int pageIndex);

// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
  // Stop further decoding as image is running off bottom of screen
  if (y >= tft.height())
    return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("setup");

  // pinMode(TFT_BL, OUTPUT);
  // digitalWrite(TFT_BL, 1);

  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK); // 清屏，设置屏幕颜色
  initTJpgDec();             // 初始化TJpg_Decode

  splashPage.onStart();
  delay(1000); // 启动页停留1秒

  pinMode(0, INPUT); // 按键
  pinMode(4, INPUT); // 按键
  pinMode(5, INPUT); // 按键
  btn1.attachClick(handleClick1);
  btn2.attachClick(handleClick2);
  btn3.attachClick(handleClick3);
  // btn.attachDoubleClick(handleDoubleClick);
  ticker.attach_ms(20, btnTick);

  // 连接wifi
  wifiPage.onStart();
  wifiPage.onLoop();
  // 同步服务器时间
  ntpClient.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // 同步时间
  ntpClient.update();
  if (millis() - lastPrintTime >= 60 * 1000) // 每分钟打印一次时间
  {
    printf("time=%s\n", ntpClient.getFormattedTime().c_str());
    lastPrintTime = millis();
  }

  Page *currentPage = getPage(currentPageIndex);
  Page *nextPage = getPage(nextPageIndex);
  if (currentPageIndex != nextPageIndex)
  {
    // 切换页面
    currentPage->onStop();
    nextPage->onStart();
    currentPageIndex = nextPageIndex;
  }
  else
  {
    currentPage->onLoop();
    delay(50);
  }
}

void btnTick()
{
  btn1.tick();
  btn2.tick();
  btn3.tick();
}

void handleClick1()
{
  // 右侧按钮，下一个
  Serial.printf("handleClick1()\n");
  nextPageIndex = currentPageIndex + 1;
  nextPageIndex = nextPageIndex % 4;
}

Page *getPage(int pageIndex)
{
  if (pageIndex < 0)
  {
    pageIndex = 0;
  }
  pageIndex = pageIndex % 4;
  switch (pageIndex)
  {
  case 0:
    return &weatherPage;
  case 1:
    return &monitorPage;
  case 2:
    return &photoPage;
  case 3:
    return &vncPage;
  }
  return &weatherPage;
}

void handleClick2()
{
  // 中间按钮
  Serial.printf("handleClick2()\n");
  ESP.reset(); // 重启系统
}

void handleClick3()
{
  // 左侧按钮，上一个
  Serial.printf("handleClick3()\n");
  nextPageIndex = currentPageIndex - 1;
  if (nextPageIndex < 0)
  {
    nextPageIndex = 3;
  }
  
}

void showClock()
{
  // tft.pushImage(0, 0, 240, 240, imgClockBackground);
  tft.setTextDatum(CC_DATUM); // 设置文本数据
  tft.setTextColor(TFT_WHITE, TFT_TRANSPARENT);
  String hms = ntpClient.getFormattedTime();
  tft.drawString(hms, 120, 100, 6);
}

void initTJpgDec()
{
  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);
  // The byte order can be swapped (set true for TFT_eSPI)
  TJpgDec.setSwapBytes(true);
  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
}