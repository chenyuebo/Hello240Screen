#include "VncPage.h"
#include <TJpg_Decoder.h>
#include "image/TouPing.h"
#include "image/TouPingIcon.h"
#include "image/Address.h"
#include "image/Port.h"
#include "image/colon.h"

void VncPage::onStart()
{
    Serial.println("VncPage::onStart()");
    _tft->fillScreen(TFT_BLACK);
    TJpgDec.drawJpg(100, 10, imgTouPingIcon, sizeof(imgTouPing));
    TJpgDec.drawJpg(95, 60, imgTouPing, sizeof(imgTouPing));
    // 默认值
    _tft->setTextDatum(TL_DATUM);
    _tft->setTextColor(TFT_WHITE, TFT_BLACK);
    // 地址
    TJpgDec.drawJpg(0, 100, imgAddress, sizeof(imgAddress));
    _tft->pushImage(60, 100, 10, 30, imgcolon); // 冒号
    _tft->drawString(WiFi.localIP().toString(), 80, 105, 4);
    // 端口
    TJpgDec.drawJpg(0, 140, imgPort, sizeof(imgPort));
    _tft->pushImage(60, 140, 10, 30, imgcolon); // 冒号
    _tft->drawString("8080", 80, 145, 4);

    resetData();
    server->begin();
}

void VncPage::onLoop()
{
    WiFiClient client = server->available();
    if (client)
    {
        client.setTimeout(1000); // 默认为 1000
        client.setNoDelay(true);
        Serial.println("new client");
        client.write("ok");
        long time = 0, timeJpg = 0;
        while (client.connected())
        {
            if (!hasSendNo)
            {
                client.write("no");
                hasSendNo = true;
            }

            while (client.available())
            {
                if (read_count == 0)
                {
                    time = millis();
                    client.read(pack_size, 2);
                    frame_size = pack_size[0] + (pack_size[1] << 8);
                    // Serial.printf("frame_size=%d\n", frame_size);
                }
                int remain_size = frame_size - size_count;
                if (remain_size >= buff_size)
                {
                    read_count = client.read(buff, buff_size);
                }
                else
                {
                    read_count = client.read(buff, remain_size);
                }

                int new_size = size_count + read_count;
                // Serial.printf("read_count=%d ,new_size=%d\n", read_count, new_size);
                if (new_size > img_buff_size)
                {
                    Serial.printf("read error read_count=%d,  new_size=%d, frame_size=%d \n", read_count, new_size, frame_size);
                    // 丢弃超过缓存区的图片帧
                    size_count = new_size;
                    if (new_size == frame_size)
                    {
                        resetData();
                        client.write("ok");
                        continue;
                    }
                }
                else
                {
                    if (frame_size <= img_buff_size)
                    {
                        memcpy(&imgBuff[size_count], buff, read_count);
                    }
                    size_count = new_size;
                }

                if (imgBuff[frame_size - 3] == 0xaa && imgBuff[frame_size - 2] == 0xbb && imgBuff[frame_size - 1] == 0xcc)
                {
                    timeJpg = millis();
                    TJpgDec.drawJpg(0, 0, imgBuff, sizeof(imgBuff) - 3);
                    timeJpg = millis() - timeJpg;
                    resetData();
                    client.write("ok");
                    time = millis() - time;
                    Serial.printf("单帧耗时=%3ld(ms), drawJpg耗时=%3ld(ms) ,帧率=%.1f\n", time, timeJpg, 1000.0 / time);
                    continue;
                }
            }
        }
        resetData();
        client.stop();
        Serial.println("client.connected()=false");
    }
}

void VncPage::resetData()
{
    hasSendNo = false;
    imgBuff[frame_size - 3] = 0;
    imgBuff[frame_size - 2] = 0;
    imgBuff[frame_size - 1] = 0;
    size_count = 0;
    read_count = 0;
}

void VncPage::onStop()
{
    Serial.println("VncPage::onStop()");
    server->close();
}