#include "MonitorPage.h"

#include "image/cpu.h"
#include "image/memory.h"
#include "image/upload.h"
#include "image/download.h"
#include "image/disk.h"
#include "image/colon.h"
#include "image/wifi_fill.h"
#include "image/wifi_off_fill.h"

void MonitorPage::onStart()
{
  Serial.println("MonitorPage::onStart()");
  // 初始化静态显示部分
  _sprite->setTextFont(4);
  _tft->fillScreen(TFT_BLACK);                                // 全屏黑色
  _tft->fillRect(0, 0, 240, 30, _tft->color565(15, 156, 83)); // 系统状态栏背景色

  // 状态栏时间
  _tft->setTextDatum(BL_DATUM); // 设置文本数据
  _tft->setTextColor(TFT_WHITE, _tft->color565(15, 156, 83));
  String hms = _ntpClient->getFormattedTime();
  _tft->drawString(hms, 10, 30, 4);

  // wifi连接状态
  if (WiFi.status() == WL_CONNECTED)
    _tft->pushImage(206, 3, 24, 24, imgwifi_fill);
  else
    _tft->pushImage(206, 3, 24, 24, imgwifi_off_fill);

  // 默认值
  _tft->setTextDatum(TL_DATUM);
  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  // CPU
  _tft->pushImage(10, 40, 50, 30, imgcpu);   // CPU
  _tft->pushImage(70, 40, 10, 30, imgcolon); // 冒号
  _tft->drawString("0%", 90, 45, 4);
  // 内存
  _tft->pushImage(10, 80, 50, 30, imgmemory); // 内存
  _tft->pushImage(70, 80, 10, 30, imgcolon);
  _tft->drawString("0%", 90, 85, 4);
  // 上传
  _tft->pushImage(10, 120, 50, 30, imgupload); // 网速上传
  _tft->pushImage(70, 120, 10, 30, imgcolon);
  _tft->drawString("0K/s", 90, 125, 4);
  // 下载
  _tft->pushImage(10, 160, 50, 30, imgdownload); // 网速下载
  _tft->pushImage(70, 160, 10, 30, imgcolon);
  _tft->drawString("0K/s", 90, 165, 4);
  // 磁盘
  _tft->pushImage(10, 200, 50, 30, imgdisk); // 磁盘占用率
  _tft->pushImage(70, 200, 10, 30, imgcolon);
  _tft->drawString("0%", 90, 205, 4);
}

void MonitorPage::drawString(int32_t x, int32_t y, const String &s)
{
  _sprite->createSprite(130, 30);
  _sprite->fillSprite(TFT_BLACK);
  _sprite->setTextDatum(CL_DATUM);
  _sprite->setTextColor(TFT_WHITE);
  _sprite->drawString(s, 0, 17);
  _sprite->pushSprite(x, y);
  _sprite->deleteSprite();
}

void MonitorPage::onLoop()
{
  // Serial.println("MonitorPage::onLoop()");
  if (_wifiClient->connected())
    readTCPData();
  else
    connectTCP();

  // wifi连接状态
  if (WiFi.status() == WL_CONNECTED)
    _tft->pushImage(206, 3, 24, 24, imgwifi_fill);
  else
    _tft->pushImage(206, 3, 24, 24, imgwifi_off_fill);

  // 不断刷新动态显示部分
  // 状态栏时间
  _tft->setTextDatum(BL_DATUM); // 设置文本数据
  _tft->setTextColor(TFT_WHITE, _tft->color565(15, 156, 83));
  String hms = _ntpClient->getFormattedTime();
  _tft->drawString(hms, 10, 30, 4);

  drawString(90, 40, cpuUsage);
  drawString(90, 80, memUsage);
  drawString(90, 120, upSpeed);
  drawString(90, 160, downSpeed);
  drawString(90, 200, ssdUsage);
}

void MonitorPage::onStop()
{
  Serial.println("MonitorPage::onStop()");
  _wifiClient->stop();
}

void MonitorPage::connectTCP()
{
  unsigned long current_time = millis();
  if (current_time - lastConnectTCPTime >= 10 * 1000) //  最小时间间隔为10秒
  {
    lastConnectTCPTime = current_time;
    Serial.printf("connect tcp\n");
    _wifiClient->setTimeout(500);
    if (_wifiClient->connect("10.235.52.17", 8022))
    {
      Serial.printf("monitor tcp connected()\n");
    }
  }
}
void MonitorPage::readTCPData()
{
  if (_wifiClient->available())
  {
    String json = _wifiClient->readStringUntil('\n');
    Serial.printf("tcp read json=%s\n", json.c_str());
    StaticJsonDocument<512> doc;
    deserializeJson(doc, json);
    JsonObject obj = doc.as<JsonObject>();
    jsonObj = obj;
    parseJson();
  }
}

void MonitorPage::parseJson()
{
  if (jsonObj)
  {
    JsonObject obj_params = jsonObj;
    if (obj_params.containsKey("ipAddr"))
    {
      String s = obj_params["ipAddr"];
      ip = s;
      Serial.printf("ipAddr=%s\n", cpuUsage.c_str());
    }
    if (obj_params.containsKey("CpuUsage"))
    {
      String s = obj_params["CpuUsage"];
      cpuUsage = s;
      Serial.printf("cpuUsage=%s\n", cpuUsage.c_str());
    }
    if (obj_params.containsKey("MemoryUsage"))
    {
      String s = obj_params["MemoryUsage"];
      memUsage = s;
      Serial.printf("MemUsage=%s\n", memUsage.c_str());
    }
    if (obj_params.containsKey("DiskUsage"))
    {
      String s = obj_params["DiskUsage"];
      ssdUsage = s;
      Serial.printf("ssdUsage=%s\n", ssdUsage.c_str());
    }
    if (obj_params.containsKey("UpSpeed"))
    {
      String s = obj_params["UpSpeed"];
      upSpeed = s;
      Serial.printf("UpSpeed=%s\n", upSpeed.c_str());
    }
    if (obj_params.containsKey("DownSpeed"))
    {
      String s = obj_params["DownSpeed"];
      downSpeed = s;
      Serial.printf("DownSpeed=%s\n", downSpeed.c_str());
    }
  }
}