#ifndef WeatherPage_H
#define WeatherPage_H

#include "Page.h"
#include "number.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "weathernum.h"

class WeatherPage : public Page
{
private:
    TFT_eSprite *_sprite;
    NTPClient *_ntpClient;
    WiFiClient *_wifiClient;
    Number number;
    int16_t bgColor = TFT_BLACK;
    long lastUpdateTime;

    String cityName;
    String temp;
    String temp_h;
    String temp_l;
    String humidity;
    String date;
    String week;
    String weather;
    String weatherCode;
    int pm25 = 0; // PM2.5空气指数

    long lastScrollTime;
    int scrollIndex = 0;
    String scrollText[7] = {"", "", "", "", "", "", ""};

    int frameIndex = 0;
    long lastAnimTime;

    void drawCityName();
    void drawBanner();
    void drawPm25();
    void drawTemp();    // 温度
    void drawHuminum(); // 湿度
    void drawClock();
    void drawDate();
    void drawAstronaut();
    void getCityWeather();

public:
    WeatherPage(TFT_eSPI &tft, TFT_eSprite &sprite, WiFiClient &wifiClient, NTPClient &ntpClient) : Page(tft)
    {
        _sprite = &sprite;
        _wifiClient = &wifiClient;
        _ntpClient = &ntpClient;
    }
    void onStart();
    void onLoop();
    void onStop();
};

#endif