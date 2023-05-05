#ifndef SplashPage_H
#define SplashPage_H

#include "Page.h"

class SplashPage : public Page
{
public:
    SplashPage(TFT_eSPI& tft): Page(tft){}
    void onStart();
};

#endif