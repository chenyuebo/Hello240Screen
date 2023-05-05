#include "PhotoAlbumPage.h"
#include <TJpg_Decoder.h>
#include "image/ClockBackground.h"
#include "image/fengjing.h"
#include "image/test.h"

void PhotoAlbumPage::onStart()
{
    Serial.println("PhotoAlbumPage::onStart()");
    _tft->fillScreen(TFT_BLACK);
    lastTime = 0;
    photoIndex = -1;
}

void PhotoAlbumPage::onLoop()
{
    if (millis() - lastTime > 5000)
    {
        lastTime = millis();
        photoIndex++;
        if (photoIndex >= 2)
        {
            photoIndex = 0;
        }
        if (photoIndex == 0)
        {
            TJpgDec.drawJpg(0, 0, imgtest, sizeof(imgtest));
            // _tft->pushImage(0, 0, 240, 240, imgtest);
        }
        else if (photoIndex == 1)
        {
            TJpgDec.drawJpg(0, 0, imgClockBackground, sizeof(imgClockBackground));
        }
    }
}

void PhotoAlbumPage::onStop()
{
    Serial.println("PhotoAlbumPage::onStop()");
}
