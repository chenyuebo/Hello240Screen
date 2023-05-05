#include "AstronautPage.h"
#include <TJpg_Decoder.h>

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

void AstronautPage::onStart()
{
    _tft->fillScreen(TFT_BLACK);
    Serial.println("AstronautPage::onStart()");
    lastTime = 0;
    frameIndex = 0;
}

void AstronautPage::onLoop()
{
    int x = 85, y = x;
    if (millis() - lastTime > 50)
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

void AstronautPage::onStop()
{
    Serial.println("AstronautPage::onStop()");
}