#ifndef AstronautPage_h
#define AstronautPage_h

#include "Page.h"

class AstronautPage : public Page
{
private:
    int frameIndex = 0;
    int lastTime;

public:
    AstronautPage(TFT_eSPI &tft) : Page(tft) {}
    void onStart();
    void onLoop();
    void onStop();
};

#endif