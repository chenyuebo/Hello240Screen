#include "WeatherPage.h"
#include <TJpg_Decoder.h>
#include <ESP8266HTTPClient.h>

#include "font/ZdyLwFont_20.h"
#include "weather/humidity.h"
#include "weather/temperature.h"
#include "weather/t0.h"

#include "astronaut/i0.h"
#include "astronaut/i1.h"
#include "astronaut/i2.h"
#include "astronaut/i3.h"
#include "astronaut/i4.h"
#include "astronaut/i5.h"
#include "astronaut/i6.h"
#include "astronaut/i7.h"
#include "astronaut/i8.h"
#include "astronaut/i9.h"

void WeatherPage::onStart()
{
    Serial.println("WeatherPage::onStart()");
    _tft->fillScreen(TFT_BLACK);
    _sprite->loadFont(ZdyLwFont_20);
    _sprite->setColorDepth(8);
    _sprite->setTextWrap(false);

    drawClock();
    TJpgDec.drawJpg(10, 183, imgtemperature, sizeof(imgtemperature)); // 温度图标
    TJpgDec.drawJpg(10, 213, imghumidity, sizeof(imghumidity));       // 湿度图标

    lastUpdateTime = LONG_MIN; // 重置时间，立即更新天气数据
    lastScrollTime = LONG_MIN;
    scrollIndex = 0;
    frameIndex = 0;
}

void WeatherPage::onLoop()
{
    getCityWeather();

    drawBanner();
    drawClock();
    drawAstronaut();
}

// 城市名称
void WeatherPage::drawCityName()
{
    _sprite->createSprite(88, 30);
    _sprite->fillSprite(bgColor);
    _sprite->setTextDatum(CC_DATUM);
    _sprite->setTextColor(TFT_WHITE, bgColor);
    _sprite->drawString(cityName, 44, 15);
    _sprite->pushSprite(10, 10);
    _sprite->deleteSprite();
}

void WeatherPage::drawBanner()
{
    if (millis() - lastScrollTime > 10 * 1000)
    {
        lastScrollTime = millis();
        if (scrollIndex > 5)
        {
            scrollIndex = 0;
        }
        String s = scrollText[scrollIndex];
        Serial.printf("drawBanner() index=%d, s=%s\n", scrollIndex, s.c_str());
        _sprite->createSprite(170, 30);
        _sprite->fillSprite(bgColor);
        _sprite->setTextDatum(CC_DATUM);
        _sprite->setTextColor(TFT_WHITE, bgColor);
        _sprite->drawString(s, 85, 15);
        _sprite->pushSprite(0, 45);
        _sprite->deleteSprite();

        scrollIndex++;
    }
}

// 空气质量
void WeatherPage::drawPm25()
{
    uint16_t pm25BgColor;
    String aqiTxt;
    int pm25V = pm25;
    if (pm25V > 300)
    {
        pm25BgColor = _tft->color565(136, 11, 32); // 严重
        aqiTxt = "严重";
    }
    else if (pm25V > 200)
    {
        pm25BgColor = _tft->color565(186, 55, 121); // 重度
        aqiTxt = "重度";
    }
    else if (pm25V > 150)
    {
        pm25BgColor = _tft->color565(219, 85, 94); // 中度
        aqiTxt = "中度";
    }
    else if (pm25V > 100)
    {
        pm25BgColor = _tft->color565(242, 159, 57); // 轻
        aqiTxt = "轻度";
    }
    else if (pm25V > 50)
    {
        pm25BgColor = _tft->color565(249, 218, 101); // 良
        aqiTxt = "良";
    }
    else
    {
        pm25BgColor = _tft->color565(156, 202, 127);
        aqiTxt = "优";
    }

    _sprite->createSprite(56, 24);
    _sprite->fillSprite(bgColor);
    _sprite->fillRoundRect(0, 0, 50, 24, 4, pm25BgColor);
    _sprite->setTextDatum(CC_DATUM);
    _sprite->setTextColor(TFT_WHITE);
    _sprite->drawString(aqiTxt, 25, 13);
    _sprite->pushSprite(104, 13);
    _sprite->deleteSprite();
}

// 温度
void WeatherPage::drawTemp()
{
    int tempnum = atoi(temp.c_str());
    int tempColor;
    if (tempnum < 10)
        tempColor = 0x00FF;
    else if (tempnum < 28)
        tempColor = 0x0AFF;
    else if (tempnum < 34)
        tempColor = 0x0F0F;
    else if (tempnum < 41)
        tempColor = 0xFF0F;
    else if (tempnum < 49)
        tempColor = 0xF00F;
    else
    {
        tempColor = 0xF00F;
        tempnum = 50;
    }

    _sprite->createSprite(56, 6);
    _sprite->fillSprite(bgColor);
    _sprite->drawRoundRect(0, 0, 52, 6, 3, TFT_WHITE);
    _sprite->fillRoundRect(1, 1, tempnum, 4, 2, tempColor);
    _sprite->pushSprite(45, 192);
    _sprite->deleteSprite();
    // 温度
    _sprite->createSprite(58, 24);
    _sprite->fillSprite(bgColor);
    _sprite->setTextDatum(CC_DATUM);
    _sprite->setTextColor(TFT_WHITE, bgColor);
    _sprite->drawString(temp + "℃", 28, 13);
    _sprite->pushSprite(100, 184);
    _sprite->deleteSprite();
}

// 湿度
void WeatherPage::drawHuminum()
{
    int huminum = atoi(humidity.c_str());
    int humiColor;
    if (huminum > 90)
        humiColor = 0x00FF;
    else if (huminum > 70)
        humiColor = 0x0AFF;
    else if (huminum > 40)
        humiColor = 0x0F0F;
    else if (huminum > 20)
        humiColor = 0xFF0F;
    else
        humiColor = 0xF00F;

    _sprite->createSprite(56, 6);
    _sprite->fillSprite(bgColor);
    _sprite->drawRoundRect(0, 0, 52, 6, 3, TFT_WHITE);
    _sprite->fillRoundRect(1, 1, huminum, 4, 2, humiColor);
    _sprite->pushSprite(45, 222);
    _sprite->deleteSprite();
    // 湿度
    _sprite->createSprite(58, 24);
    _sprite->fillSprite(bgColor);
    _sprite->setTextDatum(CC_DATUM);
    _sprite->setTextColor(TFT_WHITE, bgColor);
    _sprite->drawString(humidity + "%", 28, 13);
    _sprite->pushSprite(100, 214);
    _sprite->deleteSprite();
}

void WeatherPage::drawClock()
{
    int hour = _ntpClient->getHours();
    int minute = _ntpClient->getMinutes();
    int second = _ntpClient->getSeconds();
    int timey = 82;
    number.printfW3660(20, timey, hour / 10);
    number.printfW3660(60, timey, hour % 10);

    number.printfO3660(101, timey, minute / 10);
    number.printfO3660(141, timey, minute % 10);

    number.printfW1830(182, timey + 30, second / 10);
    number.printfW1830(202, timey + 30, second % 10);
}

void WeatherPage::drawDate()
{
    // 月日
    _sprite->createSprite(95, 30);
    _sprite->fillSprite(bgColor);
    _sprite->setTextDatum(CC_DATUM);
    _sprite->setTextColor(TFT_WHITE, bgColor);
    _sprite->drawString(date, 49, 16);
    _sprite->pushSprite(5, 150);
    _sprite->deleteSprite();
    // 星期
    _sprite->createSprite(58, 30);
    _sprite->fillSprite(bgColor);
    _sprite->setTextDatum(CC_DATUM);
    _sprite->setTextColor(TFT_WHITE, bgColor);
    _sprite->drawString(week, 29, 16);
    _sprite->pushSprite(102, 150);
    _sprite->deleteSprite();
}

void WeatherPage::getCityWeather()
{
    if (millis() - lastUpdateTime < 5 * 60 * 1000)
    {
        // 限制请求频率
        return;
    }
    lastUpdateTime = millis();

    String URL = "http://chenyuebo.cn:8080/hello240/api/weather?cityCode=101010100"; // 原来
    // 创建 HTTPClient 对象
    HTTPClient httpClient;
    httpClient.begin(*_wifiClient, URL);
    // 启动连接并发送HTTP请求
    int httpCode = httpClient.GET();
    Serial.println("正在获取天气数据");
    Serial.println(URL);

    // 如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    if (httpCode == HTTP_CODE_OK)
    {
        String str = httpClient.getString();
        Serial.println(str);
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, str);
        JsonObject json = doc.as<JsonObject>();
        JsonObject data = json["data"].as<JsonObject>();
        cityName = data["cityName"].as<String>();
        temp = data["temp"].as<String>();
        humidity = data["SD"].as<String>();
        date = data["date"].as<String>();
        week = data["week"].as<String>();
        weather = data["weather"].as<String>();
        weatherCode = data["weatherCode"].as<String>();
        String aqi_pm25 = data["aqi_pm25"].as<String>();
        pm25 = atoi(aqi_pm25.c_str());

        scrollText[0] = "实时天气 " + weather;
        scrollText[1] = "空气质量 " + aqi_pm25;
        scrollText[2] = "风向 " + data["WD"].as<String>() + data["WS"].as<String>();
        scrollText[3] = "今日 " + weather;
        scrollText[4] = "最高温度" + data["tempH"].as<String>() + "℃";
        scrollText[5] = "最低温度" + data["tempL"].as<String>() + "℃";

        // Serial.printf("cityName=%s\n", cityName.c_str());
        // Serial.printf("temp=%s\n", temp.c_str());
        // Serial.printf("date=%s\n", date.c_str());
        // Serial.printf("week=%s\n", week.c_str());
        Serial.println("获取成功");

        drawCityName();
        drawBanner();
        drawDate();
        drawPm25();
        drawTemp();
        drawHuminum();
        WeatherNum wrat;
        wrat.printfweather(175, 5, atoi(weatherCode.substring(1, 3).c_str()));
    }
    else
    {
        Serial.println("请求城市天气错误：");
        Serial.print(httpCode);
    }
    // 关闭ESP8266与服务器连接
    httpClient.end();
}

void WeatherPage::drawAstronaut()
{
    int x = 165, y = 165;
    if (millis() - lastAnimTime > 50)
    {
        frameIndex++;
        if (frameIndex > 9)
            frameIndex = 0;

        if (frameIndex == 0)
            TJpgDec.drawJpg(x, y, imgi0, sizeof(imgi0));
        else if (frameIndex == 1)
            TJpgDec.drawJpg(x, y, imgi1, sizeof(imgi1));
        else if (frameIndex == 2)
            TJpgDec.drawJpg(x, y, imgi2, sizeof(imgi2));
        else if (frameIndex == 3)
            TJpgDec.drawJpg(x, y, imgi3, sizeof(imgi3));
        else if (frameIndex == 4)
            TJpgDec.drawJpg(x, y, imgi4, sizeof(imgi4));
        else if (frameIndex == 5)
            TJpgDec.drawJpg(x, y, imgi5, sizeof(imgi5));
        else if (frameIndex == 6)
            TJpgDec.drawJpg(x, y, imgi6, sizeof(imgi6));
        else if (frameIndex == 7)
            TJpgDec.drawJpg(x, y, imgi7, sizeof(imgi7));
        else if (frameIndex == 8)
            TJpgDec.drawJpg(x, y, imgi8, sizeof(imgi8));
        else if (frameIndex == 9)
            TJpgDec.drawJpg(x, y, imgi9, sizeof(imgi9));
    }
}

void WeatherPage::onStop()
{
    Serial.println("WeatherPage::onStop()");
    _sprite->unloadFont();
}