#include <ESP8266WiFi.h>
#include "WifiConnectPage.h"

#include "image/loading1.h"
#include "image/loading2.h"
#include "image/loading3.h"
#include "image/loading4.h"
#include "image/wifi_connecting.h"

String ssid = "wifi-name";
String password = "wifi-password";

void WifiConnectPage::onStart()
{
    _tft->fillScreen(TFT_BLACK);
    _tft->pushImage(35, 160, 170, 26, imgwifi_connecting);
}

void WifiConnectPage::onLoop()
{
    Serial.printf("WiFi connecting:%s\n", ssid.c_str());
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        _tft->pushImage(90, 70, 50, 50, imgloading1);
        delay(100);
        _tft->pushImage(90, 70, 50, 50, imgloading2);
        delay(100);
        _tft->pushImage(90, 70, 50, 50, imgloading3);
        delay(100);
        _tft->pushImage(90, 70, 50, 50, imgloading4);
        delay(100);
        Serial.printf(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.printf("IP  address:%s\r\n", WiFi.localIP().toString().c_str());
    Serial.printf("mac address:%s\r\n", WiFi.macAddress().c_str());
}