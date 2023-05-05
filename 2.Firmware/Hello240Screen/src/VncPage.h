#ifndef VncPage_H
#define VncPage_H

#include "Page.h"
#include <ESP8266WiFi.h>
#include <WiFiServer.h>

class VncPage : public Page
{
protected:
    WiFiServer *server;
    uint16_t read_count = 0; // 读取buff的长度
    uint8_t pack_size[2];    // 用来装包大小字节
    uint16_t frame_size;     // 当前帧大小
    uint16_t buff_size = 2048;      // 
    uint16_t img_buff_size = 21000;  // 
    uint8_t buff[2048];
    uint8_t imgBuff[21000];
    uint16_t size_count = 0; // 计算一帧的字节大小
    bool hasSendNo = false;
    
    void resetData();

public:
    VncPage(TFT_eSPI &tft, WiFiServer &wifiServer) : Page(tft)
    {
        server = &wifiServer;
    }
    void onStart();
    void onLoop();
    void onStop();
};

#endif