#include "SplashPage.h"
#include <TJpg_Decoder.h>
#include "image/IronManCore.h"
#include "image/design_by_cyb.h"

void SplashPage::onStart()
{
    _tft->fillScreen(TFT_BLACK);
    TJpgDec.drawJpg(70, 40, imgIronManCore, sizeof(imgIronManCore));
    _tft->pushImage(45, 180, 150, 16, imgdesign_by_cyb);
}
