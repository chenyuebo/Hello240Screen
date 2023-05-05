#ifndef MonitorPage_h
#define MonitorPage_h

#include "Page.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>

class MonitorPage : public Page
{
protected:
    TFT_eSprite *_sprite;
    NTPClient *_ntpClient;
    WiFiClient *_wifiClient;
    JsonObject jsonObj;
    String ip = "192.168.0.123";
    String cpuUsage = "0%";
    String memUsage = "0%";
    String ssdUsage = "0%";
    String upSpeed = "0.0K/s";
    String downSpeed = "0.0K/s";
    unsigned long lastConnectTCPTime = 0;

public:
    MonitorPage(TFT_eSPI &tft, TFT_eSprite &sprite, WiFiClient &wifiClient, NTPClient &ntpClient) : Page(tft)
    {
        _sprite = &sprite;
        _wifiClient = &wifiClient;
        _ntpClient = &ntpClient;
    }
    void onStart();
    void onLoop();
    void onStop();

private:
    void readTCPData();
    void connectTCP();
    void parseJson();
    void drawString(int32_t x, int32_t y, const String &s);
};

#endif